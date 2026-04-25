import { describe, it, expect, beforeEach } from 'vitest';
import { useHmiStore } from '../hmiStore';
import type { AlarmEvent } from '../types';

function makeAlarm(
  id: string,
  severity: 'critical' | 'warning' | 'info' = 'warning',
  acknowledged = false,
): AlarmEvent {
  return {
    id,
    severity,
    tag:          `TAG_${id}`,
    message:      `Test alarm ${id}`,
    timestamp:    Date.now(),
    acknowledged,
  };
}

beforeEach(() => {
  // Reset Zustand store to clean initial state before every test
  useHmiStore.setState({
    navLevel:        1,
    sidebarOpen:     true,
    activeAlarms:    [],
    streamConnected: false,
    selectedTags:    [],
    timeWindow:      60,
    zoom:            null,
  });
});

// ── Navigation ─────────────────────────────────────────────────────────────

describe('hmiStore — navigation', () => {
  it('default navLevel is 1 (Overview)', () => {
    expect(useHmiStore.getState().navLevel).toBe(1);
  });

  it('setNavLevel(2) navigates to Area level', () => {
    useHmiStore.getState().setNavLevel(2);
    expect(useHmiStore.getState().navLevel).toBe(2);
  });

  it('setNavLevel(3) navigates to Detail level', () => {
    useHmiStore.getState().setNavLevel(3);
    expect(useHmiStore.getState().navLevel).toBe(3);
  });

  it('setNavLevel(1) returns to Overview from any depth', () => {
    useHmiStore.getState().setNavLevel(3);
    useHmiStore.getState().setNavLevel(1);
    expect(useHmiStore.getState().navLevel).toBe(1);
  });
});

// ── Sidebar ────────────────────────────────────────────────────────────────

describe('hmiStore — sidebar', () => {
  it('default sidebarOpen is true', () => {
    expect(useHmiStore.getState().sidebarOpen).toBe(true);
  });

  it('toggleSidebar flips true → false', () => {
    useHmiStore.getState().toggleSidebar();
    expect(useHmiStore.getState().sidebarOpen).toBe(false);
  });

  it('toggleSidebar flips false → true', () => {
    useHmiStore.getState().toggleSidebar();
    useHmiStore.getState().toggleSidebar();
    expect(useHmiStore.getState().sidebarOpen).toBe(true);
  });
});

// ── Alarms ─────────────────────────────────────────────────────────────────

describe('hmiStore — alarms', () => {
  it('activeAlarms starts empty', () => {
    expect(useHmiStore.getState().activeAlarms).toHaveLength(0);
  });

  it('addAlarm appends an alarm', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    expect(useHmiStore.getState().activeAlarms).toHaveLength(1);
  });

  it('addAlarm newest alarm appears first (prepend / LIFO)', () => {
    useHmiStore.getState().addAlarm(makeAlarm('first'));
    useHmiStore.getState().addAlarm(makeAlarm('second'));
    expect(useHmiStore.getState().activeAlarms[0].id).toBe('second');
  });

  it('activeAlarms is capped at 500 — no unbounded memory growth', () => {
    for (let i = 0; i < 600; i++) {
      useHmiStore.getState().addAlarm(makeAlarm(`a${i}`));
    }
    expect(useHmiStore.getState().activeAlarms).toHaveLength(500);
  });

  it('acknowledgeAlarm marks the target alarm acknowledged', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    useHmiStore.getState().acknowledgeAlarm('a1');
    expect(useHmiStore.getState().activeAlarms[0].acknowledged).toBe(true);
  });

  it('acknowledgeAlarm does not affect other alarms', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    useHmiStore.getState().addAlarm(makeAlarm('a2'));
    useHmiStore.getState().acknowledgeAlarm('a2');
    const a1 = useHmiStore.getState().activeAlarms.find((a) => a.id === 'a1');
    expect(a1?.acknowledged).toBe(false);
  });

  it('clearAlarms empties the list', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    useHmiStore.getState().addAlarm(makeAlarm('a2'));
    useHmiStore.getState().clearAlarms();
    expect(useHmiStore.getState().activeAlarms).toHaveLength(0);
  });
});

// ── Alarm severity priority ────────────────────────────────────────────────

describe('hmiStore — highestAlarmSeverity()', () => {
  it('returns "none" when no alarms exist', () => {
    expect(useHmiStore.getState().highestAlarmSeverity()).toBe('none');
  });

  it('returns "info" when only info alarms', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1', 'info'));
    expect(useHmiStore.getState().highestAlarmSeverity()).toBe('info');
  });

  it('returns "warning" when warning + info present', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1', 'info'));
    useHmiStore.getState().addAlarm(makeAlarm('a2', 'warning'));
    expect(useHmiStore.getState().highestAlarmSeverity()).toBe('warning');
  });

  it('returns "critical" when mixed severities are present', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1', 'info'));
    useHmiStore.getState().addAlarm(makeAlarm('a2', 'warning'));
    useHmiStore.getState().addAlarm(makeAlarm('a3', 'critical'));
    expect(useHmiStore.getState().highestAlarmSeverity()).toBe('critical');
  });

  it('ignores acknowledged alarms — critical ack → none', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1', 'critical'));
    useHmiStore.getState().acknowledgeAlarm('a1');
    expect(useHmiStore.getState().highestAlarmSeverity()).toBe('none');
  });

  it('demotes to warning when only critical is acknowledged', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1', 'critical'));
    useHmiStore.getState().addAlarm(makeAlarm('a2', 'warning'));
    useHmiStore.getState().acknowledgeAlarm('a1');
    expect(useHmiStore.getState().highestAlarmSeverity()).toBe('warning');
  });
});

// ── Waveform viewer ────────────────────────────────────────────────────────

describe('hmiStore — waveform state', () => {
  it('default timeWindow is 60 seconds', () => {
    expect(useHmiStore.getState().timeWindow).toBe(60);
  });

  it('setTimeWindow(0) means "show all" buffered data', () => {
    useHmiStore.getState().setTimeWindow(0);
    expect(useHmiStore.getState().timeWindow).toBe(0);
  });

  it('setTimeWindow updates correctly', () => {
    useHmiStore.getState().setTimeWindow(300);
    expect(useHmiStore.getState().timeWindow).toBe(300);
  });

  it('zoom starts as null', () => {
    expect(useHmiStore.getState().zoom).toBeNull();
  });

  it('setZoom stores the x-range', () => {
    useHmiStore.getState().setZoom([100, 200]);
    expect(useHmiStore.getState().zoom).toEqual([100, 200]);
  });

  it('setZoom(null) clears the zoom (reverts to auto-scale)', () => {
    useHmiStore.getState().setZoom([100, 200]);
    useHmiStore.getState().setZoom(null);
    expect(useHmiStore.getState().zoom).toBeNull();
  });

  it('streamConnected defaults to false', () => {
    expect(useHmiStore.getState().streamConnected).toBe(false);
  });

  it('setStreamConnected(true) updates correctly', () => {
    useHmiStore.getState().setStreamConnected(true);
    expect(useHmiStore.getState().streamConnected).toBe(true);
  });
});
