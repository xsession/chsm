/**
 * useHmiStore — Zustand HMI UI state
 *
 * State separation strategy
 * ─────────────────────────
 * • Zustand  owns LOCAL UI state: navigation level, sidebar, active alarms,
 *   selected tags, zoom window.  This slice never touches the network.
 *
 * • React Query (or SWR) owns SERVER state: historical data, device metadata,
 *   configuration.  Add QueryClientProvider at the app root to enable it.
 *
 * Keeping these concerns separate prevents server-refetches from triggering
 * UI re-renders and vice versa — a core principle of performant HMI design.
 */

import { create } from 'zustand';
import type { AlarmEvent, AlarmSeverity } from './types';

export type NavLevel = 1 | 2 | 3;

export interface HmiState {
  // ── Navigation level (L1 Overview → L2 Area → L3 Waveform Detail) ───────
  navLevel: NavLevel;
  setNavLevel: (l: NavLevel) => void;

  // ── Sidebar ───────────────────────────────────────────────────────────────
  sidebarOpen: boolean;
  setSidebarOpen: (v: boolean) => void;
  toggleSidebar: () => void;

  // ── Active alarms (ISA-101: unacknowledged list, capped at 500) ───────────
  activeAlarms: AlarmEvent[];
  addAlarm: (a: AlarmEvent) => void;
  acknowledgeAlarm: (id: string) => void;
  clearAlarms: () => void;

  // ── Stream connectivity indicator ─────────────────────────────────────────
  streamConnected: boolean;
  setStreamConnected: (v: boolean) => void;

  // ── Waveform viewer ───────────────────────────────────────────────────────
  /** Channels currently shown in the L3 chart panel */
  selectedTags: string[];
  setSelectedTags: (tags: string[]) => void;

  /** Visible time window in seconds; 0 = show all buffered data */
  timeWindow: number;
  setTimeWindow: (s: number) => void;

  /** Zoom x-range [min, max]; null = no zoom (pan resets to auto) */
  zoom: [number, number] | null;
  setZoom: (z: [number, number] | null) => void;

  // ── Derived helpers ───────────────────────────────────────────────────────
  /** Highest unacknowledged alarm severity — used for global status indicator */
  highestAlarmSeverity: () => AlarmSeverity;
}

export const useHmiStore = create<HmiState>((set, get) => ({
  // -- Navigation
  navLevel:    1,
  setNavLevel: (navLevel) => set({ navLevel }),

  // -- Sidebar
  sidebarOpen:    true,
  setSidebarOpen: (sidebarOpen) => set({ sidebarOpen }),
  toggleSidebar:  () => set((s) => ({ sidebarOpen: !s.sidebarOpen })),

  // -- Alarms
  activeAlarms: [],
  addAlarm: (alarm) =>
    set((s) => ({ activeAlarms: [alarm, ...s.activeAlarms].slice(0, 500) })),
  acknowledgeAlarm: (id) =>
    set((s) => ({
      activeAlarms: s.activeAlarms.map((a) =>
        a.id === id ? { ...a, acknowledged: true } : a,
      ),
    })),
  clearAlarms: () => set({ activeAlarms: [] }),

  // -- Stream
  streamConnected:    false,
  setStreamConnected: (streamConnected) => set({ streamConnected }),

  // -- Waveform viewer
  selectedTags:    [],
  setSelectedTags: (selectedTags) => set({ selectedTags }),
  timeWindow:      60,
  setTimeWindow:   (timeWindow) => set({ timeWindow }),
  zoom:            null,
  setZoom:         (zoom) => set({ zoom }),

  // -- Derived
  highestAlarmSeverity: () => {
    const unack = get().activeAlarms.filter((a) => !a.acknowledged);
    if (unack.some((a) => a.severity === 'critical')) return 'critical';
    if (unack.some((a) => a.severity === 'warning'))  return 'warning';
    if (unack.some((a) => a.severity === 'info'))     return 'info';
    return 'none';
  },
}));
