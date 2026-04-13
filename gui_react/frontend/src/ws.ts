import type { DebugLogEntry, DebugStatus } from './types';

type WsHandler = (event: string, data: Record<string, unknown>) => void;

class WsClient {
  private ws: WebSocket | null = null;
  private handlers: WsHandler[] = [];
  private reconnectTimer: ReturnType<typeof setTimeout> | null = null;

  connect() {
    if (this.ws?.readyState === WebSocket.OPEN) return;
    const proto = location.protocol === 'https:' ? 'wss' : 'ws';
    this.ws = new WebSocket(`${proto}://${location.host}/ws`);

    this.ws.onmessage = (ev) => {
      try {
        const msg = JSON.parse(ev.data);
        this.handlers.forEach((h) => h(msg.event, msg.data));
      } catch { /* ignore */ }
    };

    this.ws.onclose = () => {
      this.reconnectTimer = setTimeout(() => this.connect(), 2000);
    };

    this.ws.onerror = () => this.ws?.close();
  }

  disconnect() {
    if (this.reconnectTimer) clearTimeout(this.reconnectTimer);
    this.ws?.close();
    this.ws = null;
  }

  send(cmd: string, payload?: Record<string, unknown>) {
    if (this.ws?.readyState === WebSocket.OPEN) {
      this.ws.send(JSON.stringify({ cmd, ...payload }));
    }
  }

  on(handler: WsHandler) {
    this.handlers.push(handler);
    return () => {
      this.handlers = this.handlers.filter((h) => h !== handler);
    };
  }
}

export const wsClient = new WsClient();
