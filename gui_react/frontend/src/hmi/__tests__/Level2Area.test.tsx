import { describe, it, expect, beforeEach, vi } from 'vitest';
import { render, fireEvent } from '@testing-library/react';
import React from 'react';
import { Level2Area } from '../layout/Level2Area';
import { useHmiStore } from '../hmiStore';
import type { EquipmentStatus } from '../types';

// ── Fixtures ──────────────────────────────────────────────────────────────

const EQ_RUNNING:     EquipmentStatus = { id: 'e1', label: 'Feed Pump',      status: 'running'     };
const EQ_STOPPED:     EquipmentStatus = { id: 'e2', label: 'Discharge Valve', status: 'stopped'     };
const EQ_FAULT:       EquipmentStatus = { id: 'e3', label: 'Compressor A',    status: 'fault'       };
const EQ_MAINTENANCE: EquipmentStatus = { id: 'e4', label: 'Heat Exchanger',  status: 'maintenance' };

beforeEach(() => {
  useHmiStore.setState({
    navLevel: 2, sidebarOpen: true, activeAlarms: [],
    streamConnected: false, selectedTags: [], timeWindow: 60, zoom: null,
  });
});

// ── Accessibility & structure ──────────────────────────────────────────────

describe('Level2Area — accessibility & structure', () => {
  it('section has accessible label identifying the area level', () => {
    const { getByRole } = render(<Level2Area />);
    expect(getByRole('region', { name: /level 2/i })).toBeInTheDocument();
  });

  it('back button has aria-label for keyboard navigation', () => {
    const { getByRole } = render(<Level2Area />);
    expect(
      getByRole('button', { name: /back to system overview/i }),
    ).toBeInTheDocument();
  });

  it('equipment grid has role="list"', () => {
    const { getByRole } = render(<Level2Area equipment={[EQ_RUNNING]} />);
    expect(getByRole('list', { name: /equipment status/i })).toBeInTheDocument();
  });

  it('each equipment card has role="listitem"', () => {
    const { getAllByRole } = render(
      <Level2Area equipment={[EQ_RUNNING, EQ_STOPPED, EQ_FAULT]} />,
    );
    expect(getAllByRole('listitem')).toHaveLength(3);
  });

  it('empty equipment list shows status message instead of cards', () => {
    const { getByRole, queryAllByRole } = render(<Level2Area equipment={[]} />);
    expect(queryAllByRole('listitem')).toHaveLength(0);
    expect(getByRole('status')).toBeInTheDocument();
  });

  it('area title is rendered as a heading', () => {
    const { getByRole } = render(<Level2Area areaTitle="Reactor Area" />);
    expect(getByRole('heading', { name: 'Reactor Area' })).toBeInTheDocument();
  });

  it('equipment card aria-label encodes the status', () => {
    const { getByRole } = render(<Level2Area equipment={[EQ_FAULT]} />);
    const card = getByRole('listitem');
    expect(card).toHaveAttribute('aria-label', expect.stringContaining('fault'));
  });
});

// ── ISA-101 shape encoding (no color alone) ────────────────────────────────

describe('Level2Area — ISA-101 shape encoding (shape + text, never color alone)', () => {
  it('running equipment shows ● shape in status icon', () => {
    const { container } = render(<Level2Area equipment={[EQ_RUNNING]} />);
    const icon = container.querySelector('.hmi-equip-card__icon');
    expect(icon?.textContent).toContain('●');
  });

  it('stopped equipment shows ○ shape in status icon', () => {
    const { container } = render(<Level2Area equipment={[EQ_STOPPED]} />);
    const icon = container.querySelector('.hmi-equip-card__icon');
    expect(icon?.textContent).toContain('○');
  });

  it('fault equipment shows ◆ shape in status icon', () => {
    const { container } = render(<Level2Area equipment={[EQ_FAULT]} />);
    const icon = container.querySelector('.hmi-equip-card__icon');
    expect(icon?.textContent).toContain('◆');
  });

  it('maintenance equipment shows ▲ shape in status icon', () => {
    const { container } = render(<Level2Area equipment={[EQ_MAINTENANCE]} />);
    const icon = container.querySelector('.hmi-equip-card__icon');
    expect(icon?.textContent).toContain('▲');
  });

  it('each card also shows the status text — not shape alone', () => {
    const { container } = render(<Level2Area equipment={[EQ_FAULT]} />);
    const statusText = container.querySelector('.hmi-equip-card__status');
    expect(statusText?.textContent).toMatch(/fault/i);
  });

  it('correct CSS class applied per status for token-based theming', () => {
    const { container } = render(<Level2Area equipment={[EQ_FAULT]} />);
    expect(container.querySelector('.hmi-equip-card--fault')).toBeInTheDocument();
  });
});

// ── Alarm count badge ──────────────────────────────────────────────────────

describe('Level2Area — alarm count badge', () => {
  it('alarm count badge absent when no active alarms', () => {
    const { container } = render(<Level2Area />);
    expect(container.querySelector('.hmi-l2__alarm-count')).not.toBeInTheDocument();
  });

  it('alarm count badge appears when store has unack alarms', () => {
    useHmiStore.getState().addAlarm({
      id: 'x1', severity: 'critical', tag: 'P1', message: 'Test',
      timestamp: 0, acknowledged: false,
    });
    const { container } = render(<Level2Area />);
    expect(container.querySelector('.hmi-l2__alarm-count')).toBeInTheDocument();
  });

  it('alarm count badge has role="status" for accessible announcements', () => {
    useHmiStore.getState().addAlarm({
      id: 'x1', severity: 'warning', tag: 'T1', message: 'Test',
      timestamp: 0, acknowledged: false,
    });
    const { getByRole } = render(<Level2Area />);
    expect(getByRole('status', { name: /active alarms/i })).toBeInTheDocument();
  });
});

// ── Interaction ────────────────────────────────────────────────────────────

describe('Level2Area — interaction', () => {
  it('clicking equipment card calls onDrillDown with the equipment id', () => {
    const onDrillDown = vi.fn();
    const { container } = render(
      <Level2Area equipment={[EQ_RUNNING]} onDrillDown={onDrillDown} />,
    );
    fireEvent.click(container.querySelector('.hmi-equip-card')!);
    expect(onDrillDown).toHaveBeenCalledWith('e1');
  });

  it('back button navigates store back to navLevel 1', () => {
    const { getByRole } = render(<Level2Area />);
    fireEvent.click(getByRole('button', { name: /back to system overview/i }));
    expect(useHmiStore.getState().navLevel).toBe(1);
  });
});
