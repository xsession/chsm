import { useState } from 'react';
import { useStore } from '../store';
import { wsClient } from '../ws';

export default function DebugPanel() {
  const debugStatus = useStore((s) => s.debugStatus);
  const debugLog = useStore((s) => s.debugLog);
  const activeStates = useStore((s) => s.activeStates);
  const debugVars = useStore((s) => s.debugVars);
  const clearDebugLog = useStore((s) => s.clearDebugLog);
  const [port, setPort] = useState('9999');

  const startDebug = () => wsClient.send('debug_start', { port: parseInt(port, 10) || 9999 });
  const stopDebug = () => wsClient.send('debug_stop');

  const statusClass = debugStatus.connected
    ? 'debug-status-on'
    : debugStatus.listening
    ? 'debug-status-listening'
    : 'debug-status-off';

  const statusText = debugStatus.connected
    ? `Connected (port ${debugStatus.port})`
    : debugStatus.listening
    ? `Listening on ${debugStatus.port}...`
    : debugStatus.error
    ? `Error: ${debugStatus.error}`
    : 'Disconnected';

  return (
    <details className="debug-panel">
      <summary>Debug</summary>
      <div className="debug-content">
        <div className="debug-controls">
          <label>
            Port
            <input
              type="number"
              value={port}
              onChange={(e) => setPort(e.target.value)}
              style={{ width: 60 }}
            />
          </label>
          {!debugStatus.listening && !debugStatus.connected ? (
            <button onClick={startDebug}>Connect</button>
          ) : (
            <button onClick={stopDebug}>Stop</button>
          )}
        </div>

        <div className={`debug-status ${statusClass}`}>{statusText}</div>

        <h5>Active States</h5>
        <ul className="debug-list">
          {activeStates.map((s, i) => (
            <li key={i}>{s}</li>
          ))}
          {activeStates.length === 0 && <li className="debug-empty">—</li>}
        </ul>

        <h5>Variables</h5>
        <table className="debug-var-table">
          <thead>
            <tr><th>Name</th><th>Value</th></tr>
          </thead>
          <tbody>
            {Object.entries(debugVars).map(([k, v]) => (
              <tr key={k}>
                <td>{k}</td>
                <td>{String(v)}</td>
              </tr>
            ))}
          </tbody>
        </table>

        <h5>
          Event Log
          <button className="debug-clear-btn" onClick={clearDebugLog}>Clear</button>
        </h5>
        <ul className="debug-list debug-event-log">
          {debugLog.map((e, i) => (
            <li key={i}>
              <span className="debug-event-type">{e.type}</span>
              {e.name && <span className="debug-event-name">{e.name}</span>}
              {e.data && <span className="debug-event-data">{e.data}</span>}
            </li>
          ))}
        </ul>
      </div>
    </details>
  );
}
