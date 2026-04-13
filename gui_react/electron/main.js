/**
 * CHSM Electron — Main process.
 *
 * Spawns the FastAPI backend (either a bundled PyInstaller exe or python)
 * then loads the React frontend from the built dist/ folder.
 */
const { app, BrowserWindow, dialog, ipcMain } = require('electron');
const path = require('path');
const { spawn } = require('child_process');
const net = require('net');

const IS_DEV = !app.isPackaged;
const BACKEND_PORT = 8000;
const BACKEND_READY_TIMEOUT = 30_000; // ms

let mainWindow = null;
let backendProcess = null;

/* ── Locate the backend executable ──────────────────────────────── */
function getBackendPath() {
  if (IS_DEV) {
    // In dev mode, run python directly
    return { cmd: 'python', args: [path.join(__dirname, '..', 'backend', 'server_entry.py')] };
  }
  // In production, the PyInstaller-built exe sits in resources/backend/
  const exeName = process.platform === 'win32' ? 'chsm_backend.exe' : 'chsm_backend';
  const exePath = path.join(process.resourcesPath, 'backend', exeName);
  return { cmd: exePath, args: [] };
}

/* ── Spawn backend ─────────────────────────────────────────────── */
function startBackend() {
  const { cmd, args } = getBackendPath();
  console.log(`Starting backend: ${cmd} ${args.join(' ')}`);

  backendProcess = spawn(cmd, args, {
    stdio: ['pipe', 'pipe', 'pipe'],
    env: { ...process.env, CHSM_BACKEND_PORT: String(BACKEND_PORT) },
  });

  backendProcess.stdout.on('data', (data) => {
    process.stdout.write(`[backend] ${data}`);
  });
  backendProcess.stderr.on('data', (data) => {
    process.stderr.write(`[backend] ${data}`);
  });
  backendProcess.on('exit', (code) => {
    console.log(`Backend exited with code ${code}`);
    backendProcess = null;
  });
}

/* ── Wait for backend to be ready ──────────────────────────────── */
function waitForBackend() {
  return new Promise((resolve, reject) => {
    const start = Date.now();
    const check = () => {
      const sock = new net.Socket();
      sock.setTimeout(500);
      sock.on('connect', () => {
        sock.destroy();
        resolve();
      });
      sock.on('error', () => {
        sock.destroy();
        if (Date.now() - start > BACKEND_READY_TIMEOUT) {
          reject(new Error('Backend did not start in time'));
        } else {
          setTimeout(check, 300);
        }
      });
      sock.on('timeout', () => {
        sock.destroy();
        setTimeout(check, 300);
      });
      sock.connect(BACKEND_PORT, '127.0.0.1');
    };
    check();
  });
}

/* ── Create the main window ────────────────────────────────────── */
function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1280,
    height: 800,
    title: 'CHSM',
    icon: path.join(__dirname, 'icon.png'),
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true,
      nodeIntegration: false,
    },
  });

  // Load the React build — the backend serves it as static files
  mainWindow.loadURL(`http://127.0.0.1:${BACKEND_PORT}/`);

  if (IS_DEV) {
    mainWindow.webContents.openDevTools();
  }

  mainWindow.on('closed', () => { mainWindow = null; });
}

/* ── IPC: native file dialog ───────────────────────────────────── */
ipcMain.handle('pick-file', async (_event, opts) => {
  const mode = opts?.mode || 'open';
  const filters = opts?.filters || [{ name: 'Files', extensions: ['h', 'html'] }];
  if (mode === 'save') {
    const r = await dialog.showSaveDialog(mainWindow, { filters });
    return r.canceled ? null : r.filePath;
  }
  const r = await dialog.showOpenDialog(mainWindow, { filters, properties: ['openFile'] });
  return r.canceled ? null : r.filePaths[0];
});

ipcMain.handle('pick-directory', async () => {
  const r = await dialog.showOpenDialog(mainWindow, { properties: ['openDirectory'] });
  return r.canceled ? null : r.filePaths[0];
});

/* ── App lifecycle ─────────────────────────────────────────────── */
app.whenReady().then(async () => {
  startBackend();
  try {
    await waitForBackend();
  } catch (e) {
    console.error(e.message);
    dialog.showErrorBox('CHSM', 'Failed to start the backend server.');
    app.quit();
    return;
  }
  createWindow();
});

app.on('window-all-closed', () => {
  if (backendProcess) {
    backendProcess.kill();
    backendProcess = null;
  }
  app.quit();
});

app.on('before-quit', () => {
  if (backendProcess) {
    backendProcess.kill();
    backendProcess = null;
  }
});
