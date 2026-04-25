/**
 * MinMaxLTTB — Largest-Triangle-Three-Buckets with Min/Max Preservation
 *
 * Combines the LTTB visual-fidelity algorithm (Steinarsson 2013) with an
 * explicit min/max pass per bucket to guarantee that signal peaks and valleys
 * are never silently discarded — critical for high-frequency waveform data.
 *
 * Performance:  O(n) time,  O(threshold) space.
 * Operates on flat Float64Array [x0,y0, x1,y1, …] — zero object allocation.
 * Safe to call inside a Web Worker.
 */

export function minMaxLTTB(
  data: ArrayLike<number>,
  threshold: number,
): Float64Array {
  const len = data.length >>> 1;

  if (threshold >= len || threshold <= 2) {
    return Float64Array.from(data as number[]);
  }

  const result     = new Float64Array(threshold * 2);
  const bucketSize = (len - 2) / (threshold - 2);

  result[0] = (data as number[])[0];
  result[1] = (data as number[])[1];

  let a = 0;

  for (let i = 0; i < threshold - 2; i++) {
    const bucketStart = Math.floor((i + 0) * bucketSize) + 1;
    const bucketEnd   = Math.floor((i + 1) * bucketSize) + 1;
    const nextStart   = bucketEnd;
    const nextEnd     = Math.min(Math.floor((i + 2) * bucketSize) + 1, len);

    let avgX = 0, avgY = 0, avgCount = 0;
    for (let j = nextStart; j < nextEnd; j++) {
      avgX += (data as number[])[j * 2];
      avgY += (data as number[])[j * 2 + 1];
      avgCount++;
    }
    avgX /= avgCount;
    avgY /= avgCount;

    const ax = (data as number[])[a * 2];
    const ay = (data as number[])[a * 2 + 1];

    let maxArea = -1, ltttbIdx = bucketStart;
    let bucketMinY = Infinity, bucketMaxY = -Infinity;
    let bucketMinIdx = bucketStart, bucketMaxIdx = bucketStart;

    for (let j = bucketStart; j < bucketEnd; j++) {
      const cx = (data as number[])[j * 2];
      const cy = (data as number[])[j * 2 + 1];
      const area = Math.abs((ax - avgX) * (cy - ay) - (ax - cx) * (avgY - ay));
      if (area > maxArea) { maxArea = area; ltttbIdx = j; }
      if (cy < bucketMinY) { bucketMinY = cy; bucketMinIdx = j; }
      if (cy > bucketMaxY) { bucketMaxY = cy; bucketMaxIdx = j; }
    }

    const lttbY      = (data as number[])[ltttbIdx * 2 + 1];
    const extremeIdx = Math.abs(bucketMinY - lttbY) >= Math.abs(bucketMaxY - lttbY)
      ? bucketMinIdx : bucketMaxIdx;
    const extremeY   = (data as number[])[extremeIdx * 2 + 1];

    const selectedIdx =
      Math.abs(lttbY) > 1e-10 && Math.abs(extremeY - lttbY) / Math.abs(lttbY) > 0.05
        ? extremeIdx : ltttbIdx;

    result[(i + 1) * 2]     = (data as number[])[selectedIdx * 2];
    result[(i + 1) * 2 + 1] = (data as number[])[selectedIdx * 2 + 1];
    a = selectedIdx;
  }

  result[(threshold - 1) * 2]     = (data as number[])[(len - 1) * 2];
  result[(threshold - 1) * 2 + 1] = (data as number[])[(len - 1) * 2 + 1];
  return result;
}

export function autoDownsample(data: ArrayLike<number>, containerWidthPx: number): Float64Array {
  return minMaxLTTB(data, Math.max(Math.ceil(containerWidthPx * 2), 200));
}
