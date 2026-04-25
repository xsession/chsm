import { describe, it, expect, beforeEach, vi } from 'vitest';
import { render, fireEvent } from '@testing-library/react';
import React from 'react';
import { Level1Overview } from '../layout/Level1Overview';
import { useHmiStore } from '../hmiStore';
import type { KpiMetric, AlarmEvent } from '../types';

// ── Fixtures ──────────────────────────────────────────────────────────────

const ALARM_CRITICAL: AlarmEvent = {
  id: 'a1', severity: 'critical', tag: 'P_001',
  message: 'High pressure', timestamp: 0, acknowledged: false,
};
const ALARM_WARNING: AlarmEvent = {
  id: 'a2', severity: 'warning',  tag: 'T_001',
  message: 'High temperature', timestamp: 0, acknowledged: false,
};
const ALARM_INFO: AlarmEvent = {
  id: 'a3', severity: 'info', tag: 'F_001',
  message: 'High flow', timestamp: 0, acknowledged: false,
};

const KPI_CRITICAL: KpiMetric = { label: 'Pressure', value: 12.5, unit: 'bar', severity: 'critical' };
const KPI_WARNING:  KpiMetric = { label: 'Temp',     value: 95,   unit: '°C',  severity: 'warning'  };
const KPI_INFO:     KpiMetric = { label: 'Vibration',value: 0.8,  unit: 'g',   severity: 'info'     };
const KPI_NORMAL:   KpiMetric = { label: 'Flow',     value: 4.2,  unit: 'L/s', severity: 'none'     };

beforeEach(() => {
  useHmiStore.setState({
    navLevel: 1, sidebarOpen: true, activeAlarms: [],
    streamConnected: false, selectedTags: [], timeWindow: 60, zoom: null,
  });
});

// ── Accessibility & structure ──────────────────────────────────────────────

describe('Level1Overview — accessibility & structure', () => {
  it('section has accessible label identifying the navigation level', () => {
    const { getByRole } = render(<Level1Overview />);
    expect(getByRole('region', { name: /level 1/i })).toBeInTheDocument();
  });

  it('KPI grid has role="list" and accessible label', () => {
    const { getByRole } = render(<Level1Overview kpis={[KPI_NORMAL]} />);
    expect(getByRole('list', { name: /key process indicators/i })).toBeInTheDocument();
  });

  it('each KPI card has role="listitem"', () => {
    const { getAllByRole } = render(
      <Level1Overview kpis={[KPI_CRITICAL, KPI_WARNING, KPI_NORMAL]} />,
    );
    expect(getAllByRole('listitem')).toHaveLength(3);
  });

  it('KPI card aria-label contains the value and severity', () => {
    const { getByRole } = render(<Level1Overview kpis={[KPI_CRITICAL]} />);
    const card = getByRole('listitem');
    expect(card).toHaveAttribute('aria-label', expect.stringContaining('12.5'));
    expect(card).toHaveAttribute('aria-label', expect.stringContaining('critical'));
  });

  it('empty kpis list renders without content', () => {
    const { queryAllByRole } = render(<Level1Overview kpis={[]} />);
    expect(queryAllByRole('listitem')).toHaveLength(0);
  });
});

// ── ISA-101 shape encoding (no color alone) ────────────────────────────────

describe('Level1Overview — ISA-101 shape encoding (shape + text, never color alone)', () => {
  it('critical KPI badge contains ◆ shape', () => {
    const { container } = render(<Level1Overview kpis={[KPI_CRITICAL]} />);
    const badge = container.querySelector('.hmi-kpi-card__badge');
    expect(badge?.textContent).toContain('◆');
  });

  it('critical KPI badge also shows severity text (not color alone)', () => {
    const { container } = render(<Level1Overview kpis={[KPI_CRITICAL]} />);
    const badge = container.querySelector('.hmi-kpi-card__badge');
    expect(badge?.textContent).toMatch(/critical/i);
  });

  it('warning KPI badge contains ▲ shape', () => {
    const { container } = render(<Level1Overview kpis={[KPI_WARNING]} />);
    expect(container.querySelector('.hmi-kpi-card__badge')?.textContent).toContain('▲');
  });

  it('info KPI badge contains ● shape', () => {
    const { container } = render(<Level1Overview kpis={[KPI_INFO]} />);
    expect(container.querySelector('.hmi-kpi-card__badge')?.textContent).toContain('●');
  });

  it('normal-severity KPI has NO shape badge — visual quiet principle preserved', () => {
    const { container } = render(<Level1Overview kpis={[KPI_NORMAL]} />);
    expect(container.querySelector('.hmi-kpi-card__badge')).not.toBeInTheDocument();
  });

  it('hmi-kpi-card--critical CSS class is applied for theming', () => {
    const { container } = render(<Level1Overview kpis={[KPI_CRITICAL]} />);
    expect(container.querySelector('.hmi-kpi-card--critical')).toBeInTheDocument();
  });
});

// ── Alarm banner ───────────────────────────────────────────────────────────

describe('Level1Overview — alarm banner', () => {
  it('alarm banner is absent when no alarms exist', () => {
    const { container } = render(<Level1Overview alarms={[]} />);
    expect(container.querySelector('.hmi-alarm-banner')).not.toBeInTheDocument();
  });

  it('alarm banner renders for at least one unacknowledged alarm', () => {
    const { container } = render(<Level1Overview alarms={[ALARM_CRITICAL]} />);
    expect(container.querySelector('.hmi-alarm-banner')).toBeInTheDocument();
  });

  // KEY OVERLAP TEST: exactly one live region — no duplicate announcements
  it('exactly one aria-live="assertive" region — no duplicated banner', () => {
    const { container } = render(
      <Level1Overview alarms={[ALARM_CRITICAL, ALARM_WARNING]} />,
    );
    expect(container.querySelectorAll('[aria-live="assertive"]')).toHaveLength(1);
  });

  it('alarm banner has role="alert" for immediate screen-reader announcement', () => {
    const { container } = render(<Level1Overview alarms={[ALARM_CRITICAL]} />);
    expect(container.querySelector('[role="alert"]')).toBeInTheDocument();
  });

  it('unacknowledged alarm message text appears in the banner', () => {
    const { getByText } = render(<Level1Overview alarms={[ALARM_CRITICAL]} />);
    expect(getByText('High pressure')).toBeInTheDocument();
  });

  it('acknowledged alarm does not appear in banner', () => {
    const ack = { ...ALARM_CRITICAL, acknowledged: true };
    const { container } = render(<Level1Overview alarms={[ack]} />);
    expect(container.querySelector('.hmi-alarm-banner')).not.toBeInTheDocument();
  });

  it('alarm shape in banner row matches severity (◆ for critical)', () => {
    const { container } = render(<Level1Overview alarms={[ALARM_CRITICAL]} />);
    const shape = container.querySelector('.hmi-alarm-banner__shape');
    expect(shape?.textContent).toContain('◆');
  });

  it('banner is capped at 5 alarm entries — no overflow', () => {
    const many: AlarmEvent[] = Array.from({ length: 10 }, (_, i) => ({
      id: `a${i}`, severity: 'warning' as const, tag: `T${i}`,
      message: `Alarm ${i}`, timestamp: i, acknowledged: false,
    }));
    const { container } = render(<Level1Overview alarms={many} />);
    expect(container.querySelectorAll('.hmi-alarm-banner__item')).toHaveLength(5);
  });

  it('alarm tag is rendered in each banner item', () => {
    const { getByText } = render(<Level1Overview alarms={[ALARM_INFO]} />);
    expect(getByText('F_001')).toBeInTheDocument();
  });
});

// ── Navigation ─────────────────────────────────────────────────────────────

describe('Level1Overview — navigation', () => {
  it('nav landmark is present', () => {
    const { getByRole } = render(<Level1Overview />);
    expect(getByRole('navigation', { name: /navigate to detail views/i })).toBeInTheDocument();
  });

  it('drill-down to Equipment Area calls onDrillDown("area")', () => {
    const onDrillDown = vi.fn();
    const { getByText } = render(<Level1Overview onDrillDown={onDrillDown} />);
    fireEvent.click(getByText('Equipment Area →'));
    expect(onDrillDown).toHaveBeenCalledWith('area');
  });

  it('drill-down to Waveform calls onDrillDown("waveform")', () => {
    const onDrillDown = vi.fn();
    const { getByText } = render(<Level1Overview onDrillDown={onDrillDown} />);
    fireEvent.click(getByText('Waveform Detail →'));
    expect(onDrillDown).toHaveBeenCalledWith('waveform');
  });
});
