import { expect, afterEach } from 'vitest';
import * as matchers from '@testing-library/jest-dom/matchers';
import { cleanup } from '@testing-library/react';

// Register jest-dom custom matchers with vitest's expect
expect.extend(matchers);

// Auto-cleanup rendered components after each test (globals: false mode)
afterEach(cleanup);

// Mock ResizeObserver — not available in jsdom but used by WaveformChart
const MockResizeObserver = class {
  observe()    {}
  unobserve()  {}
  disconnect() {}
};
Object.defineProperty(globalThis, 'ResizeObserver', {
  writable:     true,
  configurable: true,
  value:        MockResizeObserver,
});

// Mock matchMedia — called by uplot CJS module at load time
Object.defineProperty(globalThis, 'matchMedia', {
  writable:     true,
  configurable: true,
  value: (query: string) => ({
    matches:             false,
    media:               query,
    onchange:            null,
    addListener:         () => {},
    removeListener:      () => {},
    addEventListener:    () => {},
    removeEventListener: () => {},
    dispatchEvent:       () => false,
  }),
});
