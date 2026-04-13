import { useEffect, useRef, useCallback } from 'react';
import { useStore } from './store';
import { wsClient } from './ws';
import { api } from './api';
import Sidebar from './components/Sidebar';
import Canvas from './components/Canvas';

export default function App() {
  const theme = useStore((s) => s.theme);
  const autosave = useStore((s) => s.autosave);
  const filepath = useStore((s) => s.filepath);
  const model = useStore((s) => s.model);
  const setMode = useStore((s) => s.setMode);
  const undo = useStore((s) => s.undo);
  const redo = useStore((s) => s.redo);
  const pushHistory = useStore((s) => s.pushHistory);
  const autosaveRef = useRef(autosave);
  autosaveRef.current = autosave;
  const filepathRef = useRef(filepath);
  filepathRef.current = filepath;
  const modelRef = useRef(model);
  modelRef.current = model;

  /* ── WebSocket ───────────────────────────────────────────────── */
  useEffect(() => {
    wsClient.connect();
    const unsub = wsClient.on((event, data) => {
      if (event === 'DEBUG_STATUS') useStore.getState().setDebugStatus(data as any);
      if (event === 'DEBUG_UPDATE') useStore.getState().handleDebugUpdate(data);
    });
    return () => { unsub(); wsClient.disconnect(); };
  }, []);

  /* ── Theme ───────────────────────────────────────────────────── */
  useEffect(() => {
    document.documentElement.setAttribute('data-theme', theme);
  }, [theme]);

  /* ── Autosave interval ───────────────────────────────────────── */
  useEffect(() => {
    if (!autosave) return;
    const id = setInterval(() => {
      const fp = filepathRef.current;
      if (fp) api.save(fp, modelRef.current as any);
    }, 5000);
    return () => clearInterval(id);
  }, [autosave]);

  /* ── Keyboard shortcuts ──────────────────────────────────────── */
  const handleKey = useCallback((e: KeyboardEvent) => {
    const tag = (e.target as HTMLElement).tagName;
    if (tag === 'INPUT' || tag === 'TEXTAREA' || tag === 'SELECT') return;

    if (e.ctrlKey || e.metaKey) {
      if (e.key === 's') { e.preventDefault(); doSave(); }
      if (e.key === 'g') { e.preventDefault(); doCodegen(); }
      if (e.key === 'z') { e.preventDefault(); undo(); }
      if (e.key === 'y') { e.preventDefault(); redo(); }
      return;
    }

    if (e.key === 's') setMode('create_state');
    if (e.key === 't') setMode('select_tr_start');
    if (e.key === 'd') setMode('delete');
    if (e.key === 'i') setMode('create_state'); // initial
    if (e.key === 'u') undo();
    if (e.key === 'r') redo();
    if (e.key === 'Escape') setMode('idle');
    if (e.key === ' ') { e.preventDefault(); useStore.getState().toggleElbow(); }
  }, [setMode, undo, redo]);

  useEffect(() => {
    window.addEventListener('keydown', handleKey);
    return () => window.removeEventListener('keydown', handleKey);
  }, [handleKey]);

  return (
    <div className="app-wrapper">
      <Sidebar onSave={doSave} onCodegen={doCodegen} onOpen={doOpen} />
      <Canvas />
    </div>
  );
}

/* ── Top-level actions ────────────────────────────────────────────── */
async function doSave() {
  const { filepath, model } = useStore.getState();
  if (filepath) {
    await api.save(filepath, model as any);
  } else {
    const pick = await api.pickFile('save', '.html');
    if (pick.ok && pick.filepath) {
      useStore.getState().setFileCtx({ filepath: pick.filepath });
      await api.save(pick.filepath, model as any);
    }
  }
}

async function doOpen() {
  const pick = await api.pickFile('open', '.h,.html');
  if (!pick.ok || !pick.filepath) return;
  const res = await api.openFile(pick.filepath);
  if (res.ok && res.model) {
    useStore.getState().setModel(res.model as any);
    useStore.getState().setFileCtx({
      filepath: res.filepath,
      filename: res.filename,
      htmlPath: (res as any).html_path ?? '',
      hFilepath: res.filepath?.endsWith('.h') ? res.filepath : '',
      fileCfg: (res as any).file_cfg ?? {},
    });
  }
}

async function doCodegen() {
  const { model, hFilepath, fileCfg } = useStore.getState();
  if (!hFilepath) {
    alert('Open a .h file first to enable code generation.');
    return;
  }
  const res = await api.codegen(model as any, hFilepath, fileCfg);
  if (res.ok) {
    alert(`Code generated:\n${res.c_file}\n${res.h_file}`);
  } else {
    alert(`Code generation failed: ${res.error}`);
  }
}
