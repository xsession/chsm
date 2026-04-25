import { describe, it, expect, beforeEach, vi } from 'vitest';
import { render, fireEvent } from '@testing-library/react';
import React from 'react';
import { Level3Detail } from '../layout/Level3Detail';
import { useHmiStore } from '../hmiStore';
import type { WaveformSeries, AlarmEvent } from '../types';

// Stub the WaveformChart so uplot is never imported in Level3Detail tests.
// The stub renders the same wrapper div (.hmi-waveform-chart) that the real
// component renders, satisfying the structural / overlap assertions below.
vi.mock('../WaveformChart', () => ({
  WaveformChart: ({ className = '' }: { series?: unknown[]; className?: string; [k: string]: unknown }) =>
    <div className={`hmi-waveform-chart${className ? ' ' + className : ''}`} />,
}));

// ── Fixtures ──────────────────────────────────────────────────────────────

const ONE_SERIES: WaveformSeries[] = [
  { label: 'Voltage', data: new Float64Array([0, 5.0, 1, 5.1, 2, 4.9]) },
];

const THREE_SERIES: WaveformSeries[] = [
  { label: 'CH1',  data: new Float64Array([0, 1, 1, 2]) },
  { label: 'CH2',  data: new Float64Array([0, 3, 1, 4]) },
  { label: 'CH3',  data: new Float64Array([0, 0, 1, 1]) },
];

function makeAlarm(id: string, severity: 'critical' | 'warning' | 'info' = 'critical'): AlarmEvent {
  return { id, severity, tag: `T_${id}`, message: `Alarm ${id}`, timestamp: 0, acknowledged: false };
}

beforeEach(() => {
  useHmiStore.setState({
    navLevel: 3, sidebarOpen: true, activeAlarms: [],
    streamConnected: false, selectedTags: [], timeWindow: 60, zoom: null,
  });
});

// ── Accessibility & structure ──────────────────────────────────────────────

describe('Level3Detail — accessibility & structure', () => {
  it('section has accessible label identifying the detail level', () => {
    const { getByRole } = render(<Level3Detail series={ONE_SERIES} />);
    expect(getByRole('region', { name: /level 3/i })).toBeInTheDocument();
  });

  it('back button is present and accessible', () => {
    const { getByRole } = render(<Level3Detail series={ONE_SERIES} />);
    expect(getByRole('button', { name: /back to area/i })).toBeInTheDocument();
  });

  it('toolbar renders as a <header> element — exactly one', () => {
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    expect(container.querySelectorAll('.hmi-l3__toolbar')).toHaveLength(1);
  });
});

// ── Time-window selector (key overlap test area) ───────────────────────────

describe('Level3Detail — time-window selector', () => {
  it('renders all 6 time-window options', () => {
    const { getByText } = render(<Level3Detail series={ONE_SERIES} />);
    for (const label of ['5 s', '10 s', '30 s', '1 m', '5 m', 'ALL']) {
      expect(getByText(label)).toBeInTheDocument();
    }
  });

  // KEY OVERLAP TEST: all time-window buttons must reside inside a single
  // .hmi-time-sel group — no buttons accidentally rendered outside or doubled.
  it('all time-window buttons are siblings inside one .hmi-time-sel group — no overlap or duplicates', () => {
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    const groups = container.querySelectorAll('.hmi-time-sel');
    expect(groups).toHaveLength(1);

    const btns = container.querySelectorAll('.hmi-time-btn');
    expect(btns).toHaveLength(6);
    btns.forEach((btn) => {
      expect(groups[0].contains(btn)).toBe(true);
    });
  });

  it('time-window button labels are all unique — no duplicate text', () => {
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    const labels = Array.from(
      container.querySelectorAll('.hmi-time-btn'),
    ).map((b) => b.textContent);
    expect(new Set(labels).size).toBe(labels.length);
  });

  it('time-window group has role="group" with accessible label', () => {
    const { getByRole } = render(<Level3Detail series={ONE_SERIES} />);
    expect(getByRole('group', { name: /time window selector/i })).toBeInTheDocument();
  });

  it('clicking a time button updates the store timeWindow', () => {
    const { getByText } = render(<Level3Detail series={ONE_SERIES} />);
    fireEvent.click(getByText('5 s'));
    expect(useHmiStore.getState().timeWindow).toBe(5);
  });

  it('clicking ALL sets timeWindow to 0 (show all data)', () => {
    const { getByText } = render(<Level3Detail series={ONE_SERIES} />);
    fireEvent.click(getByText('ALL'));
    expect(useHmiStore.getState().timeWindow).toBe(0);
  });

  it('active button has aria-pressed="true"', () => {
    useHmiStore.setState({ ...useHmiStore.getState(), timeWindow: 10 });
    const { getByText } = render(<Level3Detail series={ONE_SERIES} />);
    expect(getByText('10 s')).toHaveAttribute('aria-pressed', 'true');
  });
});

// ── Connection status ──────────────────────────────────────────────────────

describe('Level3Detail — connection status', () => {
  it('shows "LIVE" text when connected=true', () => {
    const { getByText } = render(<Level3Detail series={ONE_SERIES} connected />);
    expect(getByText('LIVE')).toBeInTheDocument();
  });

  it('shows "DISCONNECTED" text when connected=false', () => {
    const { getByText } = render(<Level3Detail series={ONE_SERIES} connected={false} />);
    expect(getByText('DISCONNECTED')).toBeInTheDocument();
  });

  it('stream status has role="status" with aria-live="polite"', () => {
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    const status = container.querySelector('[role="status"][aria-live="polite"]');
    expect(status).toBeInTheDocument();
  });
});

// ── Alarm panel ────────────────────────────────────────────────────────────

describe('Level3Detail — alarm panel', () => {
  it('alarm panel is absent when there are no unacknowledged alarms', () => {
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    expect(container.querySelector('.hmi-alarm-panel')).not.toBeInTheDocument();
  });

  it('alarm badge appears when store has unack alarms', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    expect(container.querySelector('.hmi-alarm-badge')).toBeInTheDocument();
  });

  it('alarm panel is hidden initially even with unack alarms', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    expect(container.querySelector('.hmi-alarm-panel')).not.toBeInTheDocument();
  });

  it('alarm panel becomes visible after clicking the badge toggle', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    const badge = container.querySelector('.hmi-alarm-badge') as HTMLElement;
    fireEvent.click(badge);
    expect(container.querySelector('.hmi-alarm-panel')).toBeInTheDocument();
  });

  it('alarm panel is hidden again after a second toggle click', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    const badge = container.querySelector('.hmi-alarm-badge') as HTMLElement;
    fireEvent.click(badge); // open
    fireEvent.click(badge); // close
    expect(container.querySelector('.hmi-alarm-panel')).not.toBeInTheDocument();
  });

  it('alarm badge aria-expanded reflects panel visibility', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    const badge = container.querySelector('.hmi-alarm-badge') as HTMLElement;
    expect(badge).toHaveAttribute('aria-expanded', 'false');
    fireEvent.click(badge);
    expect(badge).toHaveAttribute('aria-expanded', 'true');
  });

  it('alarm panel contains ACK button for each unack alarm', () => {
    useHmiStore.getState().addAlarm(makeAlarm('a1'));
    useHmiStore.getState().addAlarm(makeAlarm('a2'));
    const { container } = render(<Level3Detail series={ONE_SERIES} />);
    const badge = container.querySelector('.hmi-alarm-badge') as HTMLElement;
    fireEvent.click(badge);
    expect(container.querySelectorAll('.hmi-alarm-row__ack')).toHaveLength(2);
  });
});

// ── Waveform panels ────────────────────────────────────────────────────────

describe('Level3Detail — waveform panels', () => {
  it('renders exactly one WaveformChart per series channel — no overlapping panels', () => {
    const { container } = render(<Level3Detail series={THREE_SERIES} />);
    expect(container.querySelectorAll('.hmi-waveform-chart')).toHaveLength(3);
  });

  it('renders channel label text for each series', () => {
    const { getByText } = render(<Level3Detail series={THREE_SERIES} />);
    expect(getByText('CH1')).toBeInTheDocument();
    expect(getByText('CH2')).toBeInTheDocument();
    expect(getByText('CH3')).toBeInTheDocument();
  });

  it('renders each channel label exactly once — no duplicate rows', () => {
    const { container } = render(<Level3Detail series={THREE_SERIES} />);
    expect(container.querySelectorAll('.hmi-l3__chart-row')).toHaveLength(3);
  });

  it('shows "waiting for telemetry" status when series is empty', () => {
    const { container } = render(<Level3Detail series={[]} />);
    expect(container.querySelector('.hmi-l3__no-data')).toBeInTheDocument();
  });

  it('back button navigates store to navLevel 2', () => {
    const { getByRole } = render(<Level3Detail series={ONE_SERIES} />);
    fireEvent.click(getByRole('button', { name: /back to area/i }));
    expect(useHmiStore.getState().navLevel).toBe(2);
  });
});
