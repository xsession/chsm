/**
 * useWaveformStream — Real-time WebSocket telemetry hook
 *
 * Expected WebSocket message formats (JSON):
 *
 *   { "type": "TELEMETRY", "payload": { "channels": { "TAG": number }, "ts": number } }
 *   { "type": "ALARM",     "payload": AlarmEvent }
 *   { "type": "INFO",      "payload": { "sampleRate": number } }
 *
 * Reconnects automatically every 3 s on disconnect.
 * Samples are appended to a pre-allocated Float64Array ring buffer (zero GC
 * pressure during steady-state streaming) and flushed to React state via
 * requestAnimationFrame batching.
 */

import { useEffect, useRef, useCallback, useState } from 'react';
import { useHmiStore } from './hmiStore';
import type { WaveformSeries, AlarmEvent } from './types';

export interface WaveformStreamOptions {
  /** WebSocket URL, e.g. "ws://localhost:8765/ws/telemetry" */
  url: string;
  /**
   * Maximum number of (x,y) pairs kept per channel.
   * Oldest samples are evicted when this limit is reached (ring-buffer).
   * Default: 100 000
   */
  maxPoints?: number;
  /** Called once per ALARM message received from the server */
  onAlarm?: (alarm: AlarmEvent) => void;
}

export interface WaveformStreamState {
  series: WaveformSeries[];
  connected: boolean;
  error: string | null;
  /** Reported by the server via INFO message (Hz) */
  sampleRate: number;
}

export function useWaveformStream({
  url,
  maxPoints = 100_000,
  onAlarm,
}: WaveformStreamOptions): WaveformStreamState {
  const wsRef         = useRef<WebSocket | null>(null);
  const bufferRef     = useRef<Map<string, Float64Array>>(new Map());
  const usedRef       = useRef<Map<string, number>>(new Map());
  const rafRef        = useRef<number>(0);
  const dirtyRef      = useRef(false);
  const reconnTimerRef = useRef<ReturnType<typeof setTimeout>>();

  const [connected,  setConnected]  = useState(false);
  const [error,      setError]      = useState<string | null>(null);
  const [series,     setSeries]     = useState<WaveformSeries[]>([]);
  const [sampleRate, setSampleRate] = useState(0);

  const setStreamConnected = useHmiStore((s) => s.setStreamConnected);

  // ── Ring-buffer append ────────────────────────────────────────────────────
  const appendSample = useCallback(
    (tag: string, ts: number, value: number) => {
      let buf  = bufferRef.current.get(tag);
      let used = usedRef.current.get(tag) ?? 0;
      const capacity = maxPoints * 2;

      if (!buf) {
        buf = new Float64Array(capacity);
        bufferRef.current.set(tag, buf);
        used = 0;
      }

      if (used + 2 > capacity) {
        // Evict oldest 10 % of the buffer
        const shift = Math.floor(maxPoints * 0.1) * 2;
        buf.copyWithin(0, shift);
        used = capacity - shift;
      }

      buf[used]     = ts;
      buf[used + 1] = value;
      usedRef.current.set(tag, used + 2);
      dirtyRef.current = true;
    },
    [maxPoints],
  );

  // ── rAF-batched flush to React state ─────────────────────────────────────
  const scheduleFlush = useCallback(() => {
    if (rafRef.current) return;
    rafRef.current = requestAnimationFrame(() => {
      rafRef.current = 0;
      if (!dirtyRef.current) return;
      dirtyRef.current = false;

      const next: WaveformSeries[] = [];
      bufferRef.current.forEach((buf, tag) => {
        const used = usedRef.current.get(tag) ?? 0;
        // Expose a view — no copy; WaveformChart will downsample read-only
        next.push({ label: tag, data: buf.subarray(0, used) as unknown as Float64Array });
      });
      setSeries(next);
    });
  }, []);

  // ── WebSocket connection ──────────────────────────────────────────────────
  const connect = useCallback(() => {
    if (wsRef.current?.readyState === WebSocket.OPEN) return;

    const ws = new WebSocket(url);
    wsRef.current = ws;

    ws.onopen = () => {
      setConnected(true);
      setStreamConnected(true);
      setError(null);
    };

    ws.onmessage = (ev) => {
      try {
        const msg = JSON.parse(ev.data as string) as {
          type: string;
          payload: Record<string, unknown>;
        };

        if (msg.type === 'TELEMETRY') {
          const { channels, ts } = msg.payload as {
            channels: Record<string, number>;
            ts: number;
          };
          for (const [tag, val] of Object.entries(channels)) {
            appendSample(tag, ts, val);
          }
          scheduleFlush();
        } else if (msg.type === 'ALARM' && onAlarm) {
          onAlarm(msg.payload as unknown as AlarmEvent);
        } else if (msg.type === 'INFO') {
          setSampleRate((msg.payload.sampleRate as number) ?? 0);
        }
      } catch {
        /* malformed frame — silently discard */
      }
    };

    ws.onerror  = () => setError('WebSocket error');
    ws.onclose  = () => {
      setConnected(false);
      setStreamConnected(false);
      reconnTimerRef.current = setTimeout(connect, 3_000);
    };
  }, [url, appendSample, scheduleFlush, onAlarm, setStreamConnected]);

  useEffect(() => {
    connect();
    return () => {
      clearTimeout(reconnTimerRef.current);
      cancelAnimationFrame(rafRef.current);
      wsRef.current?.close();
    };
  }, [connect]);

  return { series, connected, error, sampleRate };
}
