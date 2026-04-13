import { create } from 'zustand';
import type { ChsmModel, ChsmState, ChsmTransition, ChsmConnector, EditorMode, DebugStatus, DebugLogEntry, ChsmView } from './types';

/* ── Default empty model ─────────────────────────────────────────── */
const emptyModel: ChsmModel = {
  states: {
    __top__: {
      pos: [0, 0], size: [80, 50], title: '__top__', text: [],
      connectors: [], parent: '', children: [], type: 'top',
    },
  },
  connectors: {},
  transitions: {},
  notes: {},
  view: { translate: [10, 10], scale: 10 },
};

/* ── History ─────────────────────────────────────────────────────── */
interface HistoryEntry { snapshot: string; view: ChsmView }

/* ── Store ───────────────────────────────────────────────────────── */
export interface ChsmStore {
  // ── Model ──
  model: ChsmModel;
  setModel: (m: ChsmModel) => void;
  updateState: (id: string, patch: Partial<ChsmState>) => void;
  addState: (id: string, st: ChsmState) => void;
  removeState: (id: string) => void;
  updateTransition: (id: string, patch: Partial<ChsmTransition>) => void;
  addTransition: (id: string, tr: ChsmTransition, startConn: ChsmConnector, endConn: ChsmConnector, startId: string, endId: string) => void;
  removeTransition: (id: string) => void;

  // ── View ──
  view: ChsmView;
  setView: (v: ChsmView) => void;

  // ── Editor ──
  mode: EditorMode;
  setMode: (m: EditorMode) => void;
  selectedIds: Set<string>;
  toggleSelect: (id: string) => void;
  clearSelection: () => void;

  // ── File context ──
  filepath: string;
  filename: string;
  htmlPath: string;
  hFilepath: string;
  fileCfg: Record<string, unknown>;
  setFileCtx: (ctx: { filepath?: string; filename?: string; htmlPath?: string; hFilepath?: string; fileCfg?: Record<string, unknown> }) => void;

  // ── History ──
  history: HistoryEntry[];
  historyIdx: number;
  pushHistory: () => void;
  undo: () => void;
  redo: () => void;

  // ── Debug ──
  debugStatus: DebugStatus;
  setDebugStatus: (s: DebugStatus) => void;
  debugLog: DebugLogEntry[];
  pushDebugLog: (e: DebugLogEntry) => void;
  clearDebugLog: () => void;
  activeStates: string[];
  debugVars: Record<string, unknown>;
  handleDebugUpdate: (msg: Record<string, unknown>) => void;

  // ── Elbow direction (for transition drawing) ──
  elbow: 'v' | 'h';
  toggleElbow: () => void;

  // ── Autosave ──
  autosave: boolean;
  setAutosave: (v: boolean) => void;

  // ── Theme ──
  theme: string;
  setTheme: (t: string) => void;
}

let idCounter = Date.now();
export function newId(prefix = 'obj') {
  return `${prefix}_${idCounter++}`;
}

export const useStore = create<ChsmStore>((set, get) => ({
  // ── Model ──
  model: structuredClone(emptyModel),
  setModel: (m) => {
    set({ model: m, view: m.view ?? { translate: [10, 10], scale: 10 } });
    get().pushHistory();
  },
  updateState: (id, patch) =>
    set((s) => ({
      model: {
        ...s.model,
        states: { ...s.model.states, [id]: { ...s.model.states[id], ...patch } },
      },
    })),
  addState: (id, st) =>
    set((s) => {
      const states = { ...s.model.states, [id]: st };
      if (st.parent && states[st.parent]) {
        states[st.parent] = {
          ...states[st.parent],
          children: [...states[st.parent].children, id],
        };
      }
      return { model: { ...s.model, states } };
    }),
  removeState: (id) =>
    set((s) => {
      const states = { ...s.model.states };
      const st = states[id];
      if (!st) return {};
      // Remove from parent's children
      if (st.parent && states[st.parent]) {
        states[st.parent] = {
          ...states[st.parent],
          children: states[st.parent].children.filter((c) => c !== id),
        };
      }
      // Remove connectors + transitions
      const connectors = { ...s.model.connectors };
      const transitions = { ...s.model.transitions };
      for (const cid of st.connectors) {
        const conn = connectors[cid];
        if (conn?.transition) delete transitions[conn.transition];
        delete connectors[cid];
      }
      delete states[id];
      return { model: { ...s.model, states, connectors, transitions } };
    }),
  updateTransition: (id, patch) =>
    set((s) => ({
      model: {
        ...s.model,
        transitions: { ...s.model.transitions, [id]: { ...s.model.transitions[id], ...patch } },
      },
    })),
  addTransition: (id, tr, startConn, endConn, startId, endId) =>
    set((s) => {
      const transitions = { ...s.model.transitions, [id]: tr };
      const connectors = {
        ...s.model.connectors,
        [startId]: startConn,
        [endId]: endConn,
      };
      const states = { ...s.model.states };
      const srcState = states[startConn.parent];
      const dstState = states[endConn.parent];
      if (srcState) states[startConn.parent] = { ...srcState, connectors: [...srcState.connectors, startId] };
      if (dstState) states[endConn.parent] = { ...dstState, connectors: [...dstState.connectors, endId] };
      return { model: { ...s.model, transitions, connectors, states } };
    }),
  removeTransition: (id) =>
    set((s) => {
      const transitions = { ...s.model.transitions };
      const tr = transitions[id];
      if (!tr) return {};
      const connectors = { ...s.model.connectors };
      const states = { ...s.model.states };
      for (const cid of [tr.start, tr.end]) {
        const conn = connectors[cid];
        if (conn && states[conn.parent]) {
          states[conn.parent] = {
            ...states[conn.parent],
            connectors: states[conn.parent].connectors.filter((c) => c !== cid),
          };
        }
        delete connectors[cid];
      }
      delete transitions[id];
      return { model: { ...s.model, transitions, connectors, states } };
    }),

  // ── View ──
  view: { translate: [10, 10], scale: 10 },
  setView: (v) => set({ view: v }),

  // ── Editor ──
  mode: 'idle',
  setMode: (m) => set({ mode: m }),
  selectedIds: new Set<string>(),
  toggleSelect: (id) =>
    set((s) => {
      const next = new Set(s.selectedIds);
      if (next.has(id)) next.delete(id); else next.add(id);
      return { selectedIds: next };
    }),
  clearSelection: () => set({ selectedIds: new Set() }),

  // ── File context ──
  filepath: '',
  filename: '',
  htmlPath: '',
  hFilepath: '',
  fileCfg: {},
  setFileCtx: (ctx) =>
    set((s) => ({
      filepath: ctx.filepath ?? s.filepath,
      filename: ctx.filename ?? s.filename,
      htmlPath: ctx.htmlPath ?? s.htmlPath,
      hFilepath: ctx.hFilepath ?? s.hFilepath,
      fileCfg: ctx.fileCfg ?? s.fileCfg,
    })),

  // ── History ──
  history: [],
  historyIdx: 0,
  pushHistory: () =>
    set((s) => {
      const snap = JSON.stringify(s.model);
      const entry: HistoryEntry = { snapshot: snap, view: { ...s.view } };
      const hist = s.history.slice(0, s.history.length - s.historyIdx);
      hist.push(entry);
      if (hist.length > 100) hist.shift();
      return { history: hist, historyIdx: 0 };
    }),
  undo: () =>
    set((s) => {
      const idx = s.historyIdx + 1;
      const pos = s.history.length - 1 - idx;
      if (pos < 0) return {};
      const entry = s.history[pos];
      return { model: JSON.parse(entry.snapshot), view: entry.view, historyIdx: idx };
    }),
  redo: () =>
    set((s) => {
      if (s.historyIdx <= 0) return {};
      const idx = s.historyIdx - 1;
      const pos = s.history.length - 1 - idx;
      const entry = s.history[pos];
      return { model: JSON.parse(entry.snapshot), view: entry.view, historyIdx: idx };
    }),

  // ── Debug ──
  debugStatus: { connected: false },
  setDebugStatus: (s) => set({ debugStatus: s }),
  debugLog: [],
  pushDebugLog: (e) =>
    set((s) => {
      const log = [...s.debugLog, e];
      if (log.length > 200) log.shift();
      return { debugLog: log };
    }),
  clearDebugLog: () => set({ debugLog: [], activeStates: [], debugVars: {} }),
  activeStates: [],
  debugVars: {},
  handleDebugUpdate: (msg) =>
    set((s) => {
      const t = msg.type as string;
      if (t === 'state') return { activeStates: (msg.active as string[]) ?? [] };
      if (t === 'variables') return { debugVars: (msg.vars as Record<string, unknown>) ?? {} };
      if (t === 'event') {
        const log = [...s.debugLog, msg as unknown as DebugLogEntry];
        if (log.length > 200) log.shift();
        return { debugLog: log };
      }
      if (t === 'reset') return { activeStates: [], debugVars: {}, debugLog: [] };
      return {};
    }),

  // ── Elbow ──
  elbow: 'v' as const,
  toggleElbow: () => set((s) => ({ elbow: s.elbow === 'v' ? 'h' : 'v' })),

  // ── Autosave ──
  autosave: false,
  setAutosave: (v) => set({ autosave: v }),

  // ── Theme ──
  theme: 'default',
  setTheme: (t) => set({ theme: t }),
}));
