import { useStore } from '../store';
import DebugPanel from './DebugPanel';
import NewModuleDialog from './NewModuleDialog';
import { useState } from 'react';

interface Props {
  onSave: () => void;
  onCodegen: () => void;
  onOpen: () => void;
}

export default function Sidebar({ onSave, onCodegen, onOpen }: Props) {
  const filename = useStore((s) => s.filename);
  const mode = useStore((s) => s.mode);
  const selectedIds = useStore((s) => s.selectedIds);
  const model = useStore((s) => s.model);
  const updateState = useStore((s) => s.updateState);
  const updateTransition = useStore((s) => s.updateTransition);
  const pushHistory = useStore((s) => s.pushHistory);
  const autosave = useStore((s) => s.autosave);
  const setAutosave = useStore((s) => s.setAutosave);
  const theme = useStore((s) => s.theme);
  const setTheme = useStore((s) => s.setTheme);

  const [collapsed, setCollapsed] = useState(false);
  const [showNewModule, setShowNewModule] = useState(false);

  // Get first selected state for property editing
  const selId = [...selectedIds][0] ?? null;
  const selState = selId ? model.states[selId] : null;

  // Label / text inputs
  const [label, setLabel] = useState('');
  const [text, setText] = useState('');

  const applyLabel = () => {
    if (selId && selState) { updateState(selId, { title: label }); pushHistory(); }
  };
  const applyText = () => {
    if (selId && selState) { updateState(selId, { text: text.split('\n') }); pushHistory(); }
  };

  // Sync inputs when selection changes
  const prevSel = useState<string | null>(null);
  if (selId !== prevSel[0]) {
    prevSel[1](selId);
    if (selState) { setLabel(selState.title); setText(selState.text.join('\n')); }
    else { setLabel(''); setText(''); }
  }

  return (
    <>
      <nav className={`sidebar ${collapsed ? 'sidebar-collapsed' : ''}`}>
        <div className="sidebar-content">
          <h4 className="sidebar-title">{filename || 'CHSM'}</h4>

          <div className="sidebar-buttons">
            <button onClick={() => setShowNewModule(true)}>New Module</button>
            <button onClick={onOpen}>Open</button>
            <button onClick={onCodegen}>Code Gen</button>
            <button onClick={onSave}>Save</button>
          </div>

          <hr />

          {/* Property editor */}
          <div className="prop-section">
            <div className="prop-row">
              <span className="prop-label">Label</span>
              <button className="prop-btn" onClick={applyLabel}>Apply</button>
            </div>
            <input
              type="text"
              value={label}
              onChange={(e) => setLabel(e.target.value)}
              onKeyDown={(e) => { if (e.key === 'Enter') applyLabel(); }}
              placeholder="State name"
            />
          </div>

          <div className="prop-section">
            <div className="prop-row">
              <span className="prop-label">Text</span>
              <button className="prop-btn" onClick={applyText}>Apply</button>
            </div>
            <textarea
              value={text}
              onChange={(e) => setText(e.target.value)}
              rows={4}
              placeholder="entry/ exit/ actions"
            />
          </div>

          <hr />

          {/* Settings */}
          <details>
            <summary>Settings</summary>
            <div className="settings-group">
              <label>
                <input type="checkbox" checked={autosave} onChange={(e) => setAutosave(e.target.checked)} />
                Autosave
              </label>
              <label>
                Theme
                <select value={theme} onChange={(e) => setTheme(e.target.value)}>
                  <option value="default">Default</option>
                  <option value="solarized_dark">Dark Solarized</option>
                  <option value="solarized_light">Light Solarized</option>
                </select>
              </label>
            </div>
          </details>

          {/* Help */}
          <details>
            <summary>Help</summary>
            <table className="help-table">
              <thead><tr><th>Action</th><th>Key</th></tr></thead>
              <tbody>
                <tr><td>Create state</td><td>S</td></tr>
                <tr><td>Create initial</td><td>I</td></tr>
                <tr><td>Draw transition</td><td>T</td></tr>
                <tr><td>Toggle corner</td><td>Space</td></tr>
                <tr><td>Delete object</td><td>D</td></tr>
                <tr><td>Zoom</td><td>Ctrl+Wheel</td></tr>
                <tr><td>Pan</td><td>Ctrl+Drag</td></tr>
                <tr><td>Save</td><td>Ctrl+S</td></tr>
                <tr><td>Code gen</td><td>Ctrl+G</td></tr>
                <tr><td>Undo</td><td>U / Ctrl+Z</td></tr>
                <tr><td>Redo</td><td>R / Ctrl+Y</td></tr>
              </tbody>
            </table>
          </details>

          {/* Debug */}
          <DebugPanel />
        </div>
      </nav>

      <div
        className="sidebar-handle"
        onClick={() => setCollapsed(!collapsed)}
        title="Toggle sidebar"
      >
        {collapsed ? '▶' : '◀'}
      </div>

      {showNewModule && <NewModuleDialog onClose={() => setShowNewModule(false)} />}
    </>
  );
}
