/**
 * CHSM Electron — Preload script.
 *
 * Exposes a minimal API to the renderer via contextBridge.
 * The renderer can use window.chsmElectron to invoke native dialogs.
 */
const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('chsmElectron', {
  pickFile: (opts) => ipcRenderer.invoke('pick-file', opts),
  pickDirectory: () => ipcRenderer.invoke('pick-directory'),
  isElectron: true,
});
