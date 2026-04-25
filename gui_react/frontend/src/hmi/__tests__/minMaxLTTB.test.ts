import { describe, it, expect } from 'vitest';
import { minMaxLTTB, autoDownsample } from '../minMaxLTTB';

/** Build a flat interleaved Float64Array with N (x,y) pairs: x=i, y=sin(i*0.1) */
function makeSine(n: number): Float64Array {
  const d = new Float64Array(n * 2);
  for (let i = 0; i < n; i++) {
    d[i * 2]     = i;
    d[i * 2 + 1] = Math.sin(i * 0.1);
  }
  return d;
}

describe('minMaxLTTB', () => {

  it('returns full data (as copy) when threshold >= input length', () => {
    const data   = makeSine(10);
    const result = minMaxLTTB(data, 20);
    expect(result.length).toBe(data.length);
    expect(Array.from(result)).toEqual(Array.from(data));
  });

  it('returns full data when threshold <= 2 (edge case guard)', () => {
    const data   = makeSine(100);
    const result = minMaxLTTB(data, 2);
    expect(result.length).toBe(data.length);
  });

  it('output contains exactly threshold (x,y) pairs', () => {
    const result = minMaxLTTB(makeSine(1000), 50);
    expect(result.length).toBe(50 * 2);
  });

  it('always preserves the first point — no leading gap', () => {
    const data   = makeSine(1000);
    const result = minMaxLTTB(data, 50);
    expect(result[0]).toBe(data[0]);
    expect(result[1]).toBe(data[1]);
  });

  it('always preserves the last point — no trailing truncation', () => {
    const data   = makeSine(1000);
    const result = minMaxLTTB(data, 50);
    expect(result[result.length - 2]).toBe(data[data.length - 2]);
    expect(result[result.length - 1]).toBe(data[data.length - 1]);
  });

  it('produces no NaN or Infinity values', () => {
    const result = minMaxLTTB(makeSine(1000), 50);
    for (let i = 0; i < result.length; i++) {
      expect(Number.isFinite(result[i])).toBe(true);
    }
  });

  // KEY OVERLAP TEST: downsampled x-values must never go backwards in time,
  // otherwise two chart segments could visually overlap on the canvas.
  it('x values are monotonically non-decreasing — no temporal canvas overlap', () => {
    const result = minMaxLTTB(makeSine(1000), 50);
    for (let i = 2; i < result.length; i += 2) {
      expect(result[i]).toBeGreaterThanOrEqual(result[i - 2]);
    }
  });

  it('handles empty input without throwing', () => {
    const result = minMaxLTTB(new Float64Array(0), 50);
    expect(result.length).toBe(0);
  });

  it('handles a single (x,y) point without throwing', () => {
    const data   = new Float64Array([7, 42]);
    const result = minMaxLTTB(data, 50);
    expect(result[0]).toBe(7);
    expect(result[1]).toBe(42);
  });

  it('returns a Float64Array instance', () => {
    expect(minMaxLTTB(makeSine(100), 20)).toBeInstanceOf(Float64Array);
  });
});

describe('autoDownsample', () => {

  it('returns data unchanged when point count is below 2× container width', () => {
    const data   = makeSine(10);
    const result = autoDownsample(data, 800); // threshold = 1600 >> 10
    expect(result.length).toBe(data.length);
    expect(Array.from(result)).toEqual(Array.from(data));
  });

  it('downsamples when input exceeds 2× container width', () => {
    const data        = makeSine(5000);
    const containerPx = 400;          // threshold = 800
    const result      = autoDownsample(data, containerPx);
    // 5000 pairs → should be downsampled to ≤ 800 pairs
    expect(result.length).toBeLessThanOrEqual(containerPx * 2 * 2);
  });

  it('output x values remain monotonically non-decreasing after auto-downsample', () => {
    const result = autoDownsample(makeSine(5000), 400);
    for (let i = 2; i < result.length; i += 2) {
      expect(result[i]).toBeGreaterThanOrEqual(result[i - 2]);
    }
  });

  it('returns a Float64Array', () => {
    expect(autoDownsample(makeSine(100), 400)).toBeInstanceOf(Float64Array);
  });
});
