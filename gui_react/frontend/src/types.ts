/* ── CHSM model types — mirrors the JSON schema from the Eel GUI ── */

export interface ChsmState {
  pos: [number, number];
  size: [number, number];
  title: string;
  text: string[];
  connectors: string[];
  parent: string;
  children: string[];
  type: 'normal' | 'initial' | 'top';
}

export interface ChsmConnector {
  parent: string;
  offset: number;
  side: 'left' | 'right' | 'top' | 'bottom' | 'all';
  dir: 'in' | 'out';
  transition: string;
}

export interface ChsmTransition {
  start: string;
  end: string;
  vertices: [number, number][];
  label: string;
  label_offset: [number, number];
  label_anchor: number;
  label_pos: [number, number];
}

export interface ChsmView {
  translate: [number, number];
  scale: number;
}

export interface ChsmModel {
  states: Record<string, ChsmState>;
  connectors: Record<string, ChsmConnector>;
  transitions: Record<string, ChsmTransition>;
  notes: Record<string, string>;
  view: ChsmView;
}

export type EditorMode =
  | 'idle'
  | 'create_state'
  | 'select_tr_start'
  | 'drawing_transition'
  | 'delete'
  | 'dragging'
  | 'resizing';

export interface DebugStatus {
  connected: boolean;
  listening?: boolean;
  port?: number;
  error?: string;
}

export interface DebugLogEntry {
  type: string;
  name?: string;
  data?: string;
  active?: string[];
  vars?: Record<string, unknown>;
}
