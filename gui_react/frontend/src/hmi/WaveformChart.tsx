/**
 * WaveformChart — High-Performance Waveform Renderer
 *
 * Rendering backend: uplot (MIT licence, ~40 KB, 60 FPS for 1M+ data points)
 * Data pipeline:     raw samples → MinMaxLTTB downsampling → Canvas render
 *
 * Architecture note
 * -----------------
 * The component is intentionally decoupled from the charting library through
 * the WaveformChartProps interface.  To upgrade to true WebGL acceleration,
 * replace only the `renderChart` function below:
 *
 *   • SciChart.js (commercial WebGL):
 *       Replace uPlot instantiation with SciChartSurface.create()
 *       https://www.scichart.com/documentation/js/
 *
 *   • LightningChart JS (commercial WebGL):
 *       Replace with lightningChart().ChartXY()
 *       https://lightningchart.com/js-charts/
 *
 * Install:  npm install uplot
 */

import React, { useEffect, useRef, useCallback } from 'react';
import { autoDownsample } from './minMaxLTTB';
import type { WaveformSeries } from './types';
import './WaveformChart.css';

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

export interface WaveformChartProps {
  /** Array of waveform channels to render */
  series: WaveformSeries[];
  /** Chart height in pixels (default 220) */
  height?: number;
  /** Show interactive crosshair cursor (default true) */
  crosshair?: boolean;
  /** Clip data to the last N seconds; undefined = show all buffered data */
  timeWindowSec?: number;
  /** Additional CSS class applied to the wrapper div */
  className?: string;
}

// ---------------------------------------------------------------------------
// Component
// ---------------------------------------------------------------------------

/**
 * High-performance waveform chart using uplot.
 *
 * Use MinMaxLTTB-downsampled data for input; the component performs a final
 * container-width-aware downsample pass internally to ensure the rendered
 * dataset never exceeds 2 × container pixels regardless of input size.
 */
export const WaveformChart: React.FC<WaveformChartProps> = ({
  series,
  height = 220,
  crosshair = true,
  timeWindowSec,
  className = '',
}) => {
  const containerRef = useRef<HTMLDivElement>(null);
  const chartRef     = useRef<unknown>(null);
  const cleanupRef   = useRef<(() => void) | null>(null);

  const buildChart = useCallback(async () => {
    if (!containerRef.current || series.length === 0) return;

    const container = containerRef.current;
    const width     = container.clientWidth || 800;

    // Dynamic import — uplot is code-split into its own chunk
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const uPlot = ((await import('uplot')) as any).default;

    // ── Optional time-window clipping ──────────────────────────────────────
    function clipToWindow(flat: Float64Array): Float64Array {
      if (!timeWindowSec || flat.length < 4) return flat;
      const lastTs  = flat[flat.length - 2];
      const cutoff  = lastTs - timeWindowSec;
      let startIdx  = 0;
      for (let i = 0; i < flat.length; i += 2) {
        if (flat[i] >= cutoff) { startIdx = i; break; }
      }
      return flat.subarray(startIdx);
    }

    // ── Per-series downsampling ─────────────────────────────────────────────
    const xArr: number[] = [];
    const yArrs: number[][] = series.map(() => []);

    const ds0 = autoDownsample(clipToWindow(series[0].data), width);
    for (let i = 0; i < ds0.length; i += 2) {
      xArr.push(ds0[i]);
      yArrs[0].push(ds0[i + 1]);
    }

    for (let s = 1; s < series.length; s++) {
      const ds = autoDownsample(clipToWindow(series[s].data), width);
      const ys: number[] = [];
      for (let i = 1; i < ds.length; i += 2) ys.push(ds[i]);
      yArrs[s] = ys;
    }

    // ── uplot options ───────────────────────────────────────────────────────
    const TRACE_COLORS = [
      'var(--hmi-trace-0)',
      'var(--hmi-trace-1)',
      'var(--hmi-trace-2)',
      'var(--hmi-trace-3)',
      'var(--hmi-trace-4)',
    ];

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const opts: Record<string, any> = {
      width,
      height,
      class: 'hmi-uplot',
      cursor: { show: crosshair },
      select: { show: true, stroke: 'rgba(91,156,246,0.25)', fill: 'rgba(91,156,246,0.07)' },
      legend: { show: series.length > 1 },
      scales: { x: { time: false } },
      axes: [
        {
          stroke:    '#6b7280',
          ticks:     { stroke: '#1e2229', width: 1, size: 4 },
          grid:      { stroke: '#1e2229', width: 1 },
          font:      '11px "JetBrains Mono", monospace',
          labelFont: '11px "JetBrains Mono", monospace',
        },
        {
          stroke:    '#6b7280',
          ticks:     { stroke: '#1e2229', width: 1, size: 4 },
          grid:      { stroke: '#1e2229', width: 1 },
          font:      '11px "JetBrains Mono", monospace',
          labelFont: '11px "JetBrains Mono", monospace',
          side:      3,
        },
      ],
      series: [
        // X series (hidden)
        {},
        // One entry per y-channel
        ...series.map((s, i) => ({
          label:    s.unit ? `${s.label} [${s.unit}]` : s.label,
          stroke:   s.color ?? TRACE_COLORS[i % TRACE_COLORS.length],
          width:    1.5,
          spanGaps: true,
          // Alarm threshold bands (rendered via paths hook if needed)
        })),
      ],
    };

    // ── Destroy previous instance before re-creating ────────────────────────
    cleanupRef.current?.();
    container.innerHTML = '';

    const u = new uPlot(opts, [xArr, ...yArrs], container);
    chartRef.current  = u;
    cleanupRef.current = () => u.destroy();
  }, [series, height, crosshair, timeWindowSec]);

  // ── Mount / data update ─────────────────────────────────────────────────
  useEffect(() => {
    buildChart();

    // Respond to container resize (e.g. sidebar toggle, window resize)
    const ro = new ResizeObserver(() => {
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      const u = chartRef.current as any;
      if (u && containerRef.current) {
        u.setSize({ width: containerRef.current.clientWidth, height });
      }
    });

    if (containerRef.current) ro.observe(containerRef.current);

    return () => {
      cleanupRef.current?.();
      ro.disconnect();
    };
  }, [buildChart, height]);

  return (
    <div
      ref={containerRef}
      className={`hmi-waveform-chart ${className}`}
      role="img"
      aria-label={`Waveform: ${series.map((s) => s.label).join(', ')}`}
    />
  );
};
