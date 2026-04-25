import { describe, it, expect, vi } from 'vitest';
import { render } from '@testing-library/react';
import React from 'react';
import { WaveformChart } from '../WaveformChart';
import type { WaveformSeries } from '../types';

// Mock uplot — dynamic import inside buildChart
vi.mock('uplot', () => ({
  default: class MockUPlot {
    constructor(_opts: unknown, _data: unknown, _target: unknown) {}
    destroy() {}
    setData()  {}
    setSize()  {}
  },
}));

const ONE_SERIES: WaveformSeries[] = [
  { label: 'CH1', data: new Float64Array([0, 1.0, 1, 2.0, 2, 1.5]) },
];

const TWO_SERIES: WaveformSeries[] = [
  { label: 'Voltage', data: new Float64Array([0, 5.0, 1, 5.1, 2, 4.9]) },
  { label: 'Current', data: new Float64Array([0, 1.2, 1, 1.3, 2, 1.1]) },
];

describe('WaveformChart — structure', () => {
  it('renders a container div — empty series', () => {
    const { container } = render(<WaveformChart series={[]} />);
    expect(container.querySelector('.hmi-waveform-chart')).toBeInTheDocument();
  });

  it('renders a container div — populated series', () => {
    const { container } = render(<WaveformChart series={ONE_SERIES} />);
    expect(container.querySelector('.hmi-waveform-chart')).toBeInTheDocument();
  });

  // KEY OVERLAP TEST: exactly one chart container per component instance
  it('renders exactly one container — no duplicate chart divs', () => {
    const { container } = render(<WaveformChart series={ONE_SERIES} />);
    expect(container.querySelectorAll('.hmi-waveform-chart')).toHaveLength(1);
  });

  it('container div has role="img" for screen-reader accessibility', () => {
    const { getByRole } = render(<WaveformChart series={[]} />);
    expect(getByRole('img')).toBeInTheDocument();
  });

  it('aria-label lists all channel names', () => {
    const { getByRole } = render(<WaveformChart series={TWO_SERIES} />);
    expect(getByRole('img')).toHaveAttribute(
      'aria-label',
      'Waveform: Voltage, Current',
    );
  });

  it('second render replaces — still exactly one container', () => {
    const { container, rerender } = render(<WaveformChart series={[]} />);
    rerender(<WaveformChart series={ONE_SERIES} />);
    expect(container.querySelectorAll('.hmi-waveform-chart')).toHaveLength(1);
  });
});

describe('WaveformChart — props forwarding', () => {
  it('applies additional className without losing base class', () => {
    const { container } = render(
      <WaveformChart series={[]} className="my-overlay" />,
    );
    const el = container.querySelector('.hmi-waveform-chart');
    expect(el).toHaveClass('my-overlay');
    expect(el).toHaveClass('hmi-waveform-chart');
  });

  it('does not throw when timeWindowSec is provided', () => {
    expect(() =>
      render(<WaveformChart series={ONE_SERIES} timeWindowSec={30} />),
    ).not.toThrow();
  });

  it('does not throw when timeWindowSec is undefined', () => {
    expect(() =>
      render(<WaveformChart series={ONE_SERIES} timeWindowSec={undefined} />),
    ).not.toThrow();
  });
});
