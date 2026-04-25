/**
 * Level1Overview — ISA-101 Level 1 (Overview Screen)
 *
 * Shows:
 *   • KPI card grid   — key process indicators with severity-coded state
 *   • Alarm banner    — top 5 unacknowledged alarms (live, aria-live)
 *   • Navigation      — drill-down buttons to L2 / L3
 *
 * ISA-101 rule enforced:
 *   Alarm state is communicated via BOTH color AND shape/text so the
 *   display remains unambiguous for users with color-vision deficiency.
 */

import React from 'react';
import { useHmiStore } from '../hmiStore';
import type { KpiMetric, AlarmEvent } from '../types';
import './HmiLayout.css';

interface Level1Props {
  kpis?: KpiMetric[];
  /** Override alarms (defaults to store) */
  alarms?: AlarmEvent[];
  onDrillDown?: (destination: 'area' | 'waveform') => void;
}

// Shape tokens: color alone is never used as the only indicator (ISA-101 §5.4)
const ALARM_SHAPE: Record<string, string> = {
  critical: '◆',
  warning:  '▲',
  info:     '●',
};

export const Level1Overview: React.FC<Level1Props> = ({
  kpis = [],
  alarms,
  onDrillDown,
}) => {
  const storeAlarms = useHmiStore((s) => s.activeAlarms);
  const setNavLevel = useHmiStore((s) => s.setNavLevel);
  const displayed   = alarms ?? storeAlarms;
  const unack       = displayed.filter((a) => !a.acknowledged);

  return (
    <section className="hmi-l1" aria-label="Level 1 — System Overview">

      {/* ── KPI card grid ─────────────────────────────────────────────────── */}
      <div
        className="hmi-kpi-grid"
        role="list"
        aria-label="Key Process Indicators"
      >
        {kpis.map((kpi) => (
          <article
            key={kpi.label}
            role="listitem"
            className={`hmi-kpi-card hmi-kpi-card--${kpi.severity}`}
            aria-label={`${kpi.label}: ${kpi.value}${kpi.unit ? ' ' + kpi.unit : ''}, severity ${kpi.severity}`}
          >
            <span className="hmi-kpi-card__label">{kpi.label}</span>
            <span className="hmi-kpi-card__value">
              {kpi.value}
              {kpi.unit && (
                <span className="hmi-kpi-card__unit"> {kpi.unit}</span>
              )}
            </span>
            {kpi.severity !== 'none' && (
              <span className="hmi-kpi-card__badge" aria-hidden="true">
                {ALARM_SHAPE[kpi.severity]} {kpi.severity.toUpperCase()}
              </span>
            )}
            {kpi.trend && (
              <span
                className="hmi-kpi-card__trend"
                aria-label={`Trend: ${kpi.trend}`}
              >
                {kpi.trend === 'up' ? '↑' : kpi.trend === 'down' ? '↓' : '→'}
              </span>
            )}
          </article>
        ))}
      </div>

      {/* ── Active alarm banner ────────────────────────────────────────────── */}
      {unack.length > 0 && (
        <div
          className="hmi-alarm-banner"
          role="alert"
          aria-live="assertive"
          aria-atomic="false"
        >
          <span className="hmi-alarm-banner__count" aria-hidden="true">
            {ALARM_SHAPE[unack[0].severity]} {unack.length} ACTIVE
          </span>
          <ul className="hmi-alarm-banner__list">
            {unack.slice(0, 5).map((a) => (
              <li
                key={a.id}
                className={`hmi-alarm-banner__item hmi-alarm-banner__item--${a.severity}`}
              >
                <span className="hmi-alarm-banner__shape" aria-hidden="true">
                  {ALARM_SHAPE[a.severity]}
                </span>
                <span className="hmi-alarm-banner__tag">{a.tag}</span>
                <span className="hmi-alarm-banner__msg">{a.message}</span>
              </li>
            ))}
          </ul>
        </div>
      )}

      {/* ── Navigation ──────────────────────────────────────────────────────── */}
      <nav className="hmi-l1__nav" aria-label="Navigate to detail views">
        <button
          className="hmi-nav-btn"
          onClick={() => { setNavLevel(2); onDrillDown?.('area'); }}
        >
          Equipment Area →
        </button>
        <button
          className="hmi-nav-btn"
          onClick={() => { setNavLevel(3); onDrillDown?.('waveform'); }}
        >
          Waveform Detail →
        </button>
      </nav>
    </section>
  );
};
