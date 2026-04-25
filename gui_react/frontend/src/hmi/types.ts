/** Shared HMI data types — used across all hmi/* modules */

export interface WaveformSeries {
  label: string;
  /** Flat interleaved [x0,y0, x1,y1, …] */
  data: Float64Array;
  unit?: string;
  color?: string;
  alarmHigh?: number;
  alarmLow?: number;
}

export type AlarmSeverity = 'none' | 'info' | 'warning' | 'critical';

export interface AlarmEvent {
  id: string;
  severity: AlarmSeverity;
  tag: string;
  message: string;
  timestamp: number;
  acknowledged: boolean;
}

export interface KpiMetric {
  label: string;
  value: number | string;
  unit?: string;
  severity: AlarmSeverity;
  trend?: 'up' | 'down' | 'stable';
}

export interface EquipmentStatus {
  id: string;
  label: string;
  status: 'running' | 'stopped' | 'fault' | 'maintenance';
  value?: string;
  unit?: string;
}
