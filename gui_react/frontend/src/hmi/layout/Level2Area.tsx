/**
 * Level2Area — ISA-101 Level 2 (Area / Equipment view)
 *
 * Shows individual equipment / process units as interactive status cards.
 * Clicking any card drills through to the L3 waveform detail for that unit.
 *
 * ISA-101 rule enforced:
 *   Equipment status is communicated via shape (●○◆▲) + text label,
 *   NOT by color alone.
 */

import React from 'react';
import { useHmiStore } from '../hmiStore';
import type { EquipmentStatus } from '../types';
import './HmiLayout.css';

interface Level2Props {
  equipment?: EquipmentStatus[];
  areaTitle?: string;
  onDrillDown?: (equipmentId: string) => void;
}

const STATUS_SHAPE: Record<string, string> = {
  running:     '●',
  stopped:     '○',
  fault:       '◆',
  maintenance: '▲',
};

export const Level2Area: React.FC<Level2Props> = ({
  equipment = [],
  areaTitle = 'Area',
  onDrillDown,
}) => {
  const setNavLevel  = useHmiStore((s) => s.setNavLevel);
  const alarmCount   = useHmiStore((s) =>
    s.activeAlarms.filter((a) => !a.acknowledged).length,
  );

  return (
    <section className="hmi-l2" aria-label="Level 2 — Equipment Area">

      {/* ── Header ──────────────────────────────────────────────────────── */}
      <header className="hmi-l2__header">
        <button
          className="hmi-nav-btn hmi-nav-btn--back"
          onClick={() => setNavLevel(1)}
          aria-label="Back to system overview"
        >
          ← Overview
        </button>

        <h2 className="hmi-l2__title">{areaTitle}</h2>

        {alarmCount > 0 && (
          <span
            className="hmi-l2__alarm-count hmi-l2__alarm-count--critical"
            role="status"
            aria-label={`${alarmCount} active alarms`}
          >
            ◆ {alarmCount} ACTIVE
          </span>
        )}
      </header>

      {/* ── Equipment grid ──────────────────────────────────────────────── */}
      <div className="hmi-equip-grid" role="list" aria-label="Equipment status">
        {equipment.map((eq) => (
          <button
            key={eq.id}
            role="listitem"
            className={`hmi-equip-card hmi-equip-card--${eq.status}`}
            onClick={() => { setNavLevel(3); onDrillDown?.(eq.id); }}
            aria-label={`${eq.label}, status: ${eq.status}${eq.value != null ? ', ' + eq.value + (eq.unit ? ' ' + eq.unit : '') : ''}`}
          >
            <span className="hmi-equip-card__icon" aria-hidden="true">
              {STATUS_SHAPE[eq.status]}
            </span>
            <span className="hmi-equip-card__label">{eq.label}</span>
            <span className="hmi-equip-card__status">{eq.status.toUpperCase()}</span>
            {eq.value != null && (
              <span className="hmi-equip-card__value">
                {eq.value}
                {eq.unit && <span className="hmi-equip-card__unit"> {eq.unit}</span>}
              </span>
            )}
          </button>
        ))}

        {equipment.length === 0 && (
          <p className="hmi-l2__empty" role="status">
            No equipment defined for this area.
          </p>
        )}
      </div>
    </section>
  );
};
