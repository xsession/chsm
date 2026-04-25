/**
 * Level3Detail — ISA-101 Level 3 (Waveform / Diagnostics Detail)
 *
 * The core Digilent-style waveform viewer:
 *   • Live stream status indicator
 *   • Time-window selector (5s / 10s / 30s / 1m / 5m / ALL)
 *   • Alarm panel (expandable; alarm count badge on toolbar)
 *   • Per-channel WaveformChart panels with trace swatch + label
 *
 * Data flow:
 *   useWaveformStream → WaveformSeries[] → MinMaxLTTB (inside WaveformChart)
 *   → uplot Canvas render at 60 FPS
 */

import React, { useState } from 'react';
import { useHmiStore } from '../hmiStore';
import { WaveformChart } from '../WaveformChart';
import type { WaveformSeries } from '../types';
import './HmiLayout.css';

interface Level3Props {
  series: WaveformSeries[];
  connected?: boolean;
  sampleRate?: number;
  channelTitle?: string;
}

const TIME_WINDOWS = [5, 10, 30, 60, 300, 0] as const;
const TIME_LABELS  = ['5 s', '10 s', '30 s', '1 m', '5 m', 'ALL'];

const TRACE_COLORS = [
  'var(--hmi-trace-0)',
  'var(--hmi-trace-1)',
  'var(--hmi-trace-2)',
  'var(--hmi-trace-3)',
  'var(--hmi-trace-4)',
];

export const Level3Detail: React.FC<Level3Props> = ({
  series,
  connected = false,
  sampleRate = 0,
  channelTitle = 'Waveform Detail',
}) => {
  const setNavLevel      = useHmiStore((s) => s.setNavLevel);
  const timeWindow       = useHmiStore((s) => s.timeWindow);
  const setTimeWindow    = useHmiStore((s) => s.setTimeWindow);
  const unackAlarms      = useHmiStore((s) => s.activeAlarms.filter((a) => !a.acknowledged));
  const acknowledgeAlarm = useHmiStore((s) => s.acknowledgeAlarm);

  const [alarmPanelOpen, setAlarmPanelOpen] = useState(false);

  return (
    <section className="hmi-l3" aria-label="Level 3 — Waveform Detail">

      {/* ── Toolbar ─────────────────────────────────────────────────────── */}
      <header className="hmi-l3__toolbar">
        <button
          className="hmi-nav-btn hmi-nav-btn--back"
          onClick={() => setNavLevel(2)}
          aria-label="Back to area view"
        >
          ← Area
        </button>

        <h2 className="hmi-l3__title">{channelTitle}</h2>

        {/* Connection / sample-rate badge */}
        <div
          className="hmi-l3__stream-status"
          role="status"
          aria-live="polite"
          aria-label={connected ? `Live stream at ${sampleRate} Hz` : 'Stream disconnected'}
        >
          <span
            className={`hmi-status-dot hmi-status-dot--${connected ? 'ok' : 'fault'}`}
            aria-hidden="true"
          />
          <span className="hmi-l3__stream-label">
            {connected ? 'LIVE' : 'DISCONNECTED'}
          </span>
          {sampleRate > 0 && (
            <span className="hmi-l3__sample-rate">{sampleRate} Hz</span>
          )}
        </div>

        {/* Time-window selector */}
        <div
          className="hmi-time-sel"
          role="group"
          aria-label="Time window selector"
        >
          {TIME_WINDOWS.map((w, i) => (
            <button
              key={w}
              className={`hmi-time-btn${timeWindow === w ? ' hmi-time-btn--active' : ''}`}
              onClick={() => setTimeWindow(w)}
              aria-pressed={timeWindow === w}
            >
              {TIME_LABELS[i]}
            </button>
          ))}
        </div>

        {/* Alarm count badge — shape + count, never color only */}
        {unackAlarms.length > 0 && (
          <button
            className="hmi-alarm-badge hmi-alarm-badge--critical"
            onClick={() => setAlarmPanelOpen((v) => !v)}
            aria-expanded={alarmPanelOpen}
            aria-label={`${unackAlarms.length} unacknowledged alarms — click to ${alarmPanelOpen ? 'hide' : 'show'}`}
          >
            ◆ {unackAlarms.length}
          </button>
        )}
      </header>

      {/* ── Inline alarm panel (expandable) ─────────────────────────────── */}
      {alarmPanelOpen && unackAlarms.length > 0 && (
        <div
          className="hmi-alarm-panel"
          role="region"
          aria-label="Unacknowledged alarms"
        >
          {unackAlarms.map((a) => (
            <div
              key={a.id}
              className={`hmi-alarm-row hmi-alarm-row--${a.severity}`}
            >
              <span className="hmi-alarm-row__shape" aria-hidden="true">
                {a.severity === 'critical' ? '◆' : a.severity === 'warning' ? '▲' : '●'}
              </span>
              <span className="hmi-alarm-row__tag">{a.tag}</span>
              <span className="hmi-alarm-row__msg">{a.message}</span>
              <button
                className="hmi-alarm-row__ack"
                onClick={() => acknowledgeAlarm(a.id)}
                aria-label={`Acknowledge ${a.tag}`}
              >
                ACK
              </button>
            </div>
          ))}
        </div>
      )}

      {/* ── Waveform panels ─────────────────────────────────────────────── */}
      <div className="hmi-l3__charts">
        {series.length === 0 ? (
          <p
            className="hmi-l3__no-data"
            role="status"
            aria-live="polite"
          >
            Waiting for telemetry data…
          </p>
        ) : (
          series.map((s, i) => (
            <div key={s.label} className="hmi-l3__chart-row">
              <div
                className="hmi-l3__chart-label"
                aria-label={`Channel: ${s.label}${s.unit ? ` (${s.unit})` : ''}`}
              >
                <span
                  className="hmi-trace-swatch"
                  style={{ background: s.color ?? TRACE_COLORS[i % TRACE_COLORS.length] }}
                  aria-hidden="true"
                />
                <span className="hmi-l3__tag-name">{s.label}</span>
                {s.unit && (
                  <span className="hmi-text-secondary"> ({s.unit})</span>
                )}
              </div>
              <WaveformChart
                series={[s]}
                height={180}
                crosshair
                timeWindowSec={timeWindow || undefined}
              />
            </div>
          ))
        )}
      </div>
    </section>
  );
};
