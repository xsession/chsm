import { useRef, useCallback, useState } from 'react';
import { useStore } from '../store';
import type { ChsmState, ChsmView } from '../types';

interface Props {
  id: string;
  state: ChsmState;
  isActive: boolean;
  svgRef: React.RefObject<SVGSVGElement | null>;
  view: ChsmView;
}

export default function StateNode({ id, state, isActive, svgRef, view }: Props) {
  const updateState = useStore((s) => s.updateState);
  const pushHistory = useStore((s) => s.pushHistory);
  const mode = useStore((s) => s.mode);
  const selectedIds = useStore((s) => s.selectedIds);
  const toggleSelect = useStore((s) => s.toggleSelect);
  const isSelected = selectedIds.has(id);

  const [dragging, setDragging] = useState(false);
  const [resizing, setResizing] = useState(false);
  const dragStart = useRef<{ mx: number; my: number; ox: number; oy: number } | null>(null);
  const resizeStart = useRef<{ mx: number; my: number; ow: number; oh: number } | null>(null);

  const [editing, setEditing] = useState<'title' | 'text' | null>(null);
  const [editValue, setEditValue] = useState('');

  const toSvg = useCallback(
    (cx: number, cy: number): [number, number] => {
      if (!svgRef.current) return [0, 0];
      const rect = svgRef.current.getBoundingClientRect();
      return [
        (cx - rect.left - view.translate[0]) / view.scale,
        (cy - rect.top - view.translate[1]) / view.scale,
      ];
    },
    [svgRef, view],
  );

  /* ── Drag ────────────────────────────────────────────────────── */
  const onDragStart = useCallback(
    (e: React.MouseEvent) => {
      if (mode !== 'idle' && mode !== 'dragging') return;
      if (e.ctrlKey) { toggleSelect(id); return; }
      e.stopPropagation();
      const [mx, my] = toSvg(e.clientX, e.clientY);
      dragStart.current = { mx, my, ox: state.pos[0], oy: state.pos[1] };
      setDragging(true);

      const onMove = (ev: MouseEvent) => {
        if (!dragStart.current) return;
        const [nx, ny] = toSvg(ev.clientX, ev.clientY);
        const dx = nx - dragStart.current.mx;
        const dy = ny - dragStart.current.my;
        updateState(id, { pos: [dragStart.current.ox + dx, dragStart.current.oy + dy] });
      };

      const onUp = () => {
        window.removeEventListener('mousemove', onMove);
        window.removeEventListener('mouseup', onUp);
        setDragging(false);
        pushHistory();
      };

      window.addEventListener('mousemove', onMove);
      window.addEventListener('mouseup', onUp);
    },
    [id, mode, state.pos, toSvg, updateState, pushHistory, toggleSelect],
  );

  /* ── Resize ──────────────────────────────────────────────────── */
  const onResizeStart = useCallback(
    (e: React.MouseEvent) => {
      e.stopPropagation();
      const [mx, my] = toSvg(e.clientX, e.clientY);
      resizeStart.current = { mx, my, ow: state.size[0], oh: state.size[1] };
      setResizing(true);

      const onMove = (ev: MouseEvent) => {
        if (!resizeStart.current) return;
        const [nx, ny] = toSvg(ev.clientX, ev.clientY);
        const dw = nx - resizeStart.current.mx;
        const dh = ny - resizeStart.current.my;
        updateState(id, {
          size: [
            Math.max(5, resizeStart.current.ow + dw),
            Math.max(3, resizeStart.current.oh + dh),
          ],
        });
      };

      const onUp = () => {
        window.removeEventListener('mousemove', onMove);
        window.removeEventListener('mouseup', onUp);
        setResizing(false);
        pushHistory();
      };

      window.addEventListener('mousemove', onMove);
      window.addEventListener('mouseup', onUp);
    },
    [id, state.size, toSvg, updateState, pushHistory],
  );

  /* ── Inline edit ─────────────────────────────────────────────── */
  const onDoubleClick = useCallback(
    (e: React.MouseEvent) => {
      e.stopPropagation();
      setEditing('title');
      setEditValue(state.title);
    },
    [state.title],
  );

  const commitEdit = useCallback(() => {
    if (editing === 'title') updateState(id, { title: editValue });
    if (editing === 'text') updateState(id, { text: editValue.split('\n') });
    setEditing(null);
    pushHistory();
  }, [editing, editValue, id, updateState, pushHistory]);

  const [x, y] = state.pos;
  const [w, h] = state.size;
  const isInitial = state.type === 'initial';
  const titleHeight = 2;

  return (
    <g transform={`translate(${x}, ${y})`}>
      {/* Body */}
      <rect
        width={w}
        height={h}
        rx={isInitial ? w / 2 : 0.5}
        ry={isInitial ? h / 2 : 0.5}
        className={`state-body ${isActive ? 'state-active' : ''} ${isSelected ? 'state-selected' : ''}`}
      />

      {/* Title separator */}
      {!isInitial && <line x1={0} y1={titleHeight} x2={w} y2={titleHeight} className="state-separator" />}

      {/* Title */}
      {editing === 'title' ? (
        <foreignObject x={0.2} y={0.2} width={w - 0.4} height={titleHeight - 0.2}>
          <input
            autoFocus
            className="state-inline-edit"
            value={editValue}
            onChange={(e) => setEditValue(e.target.value)}
            onBlur={commitEdit}
            onKeyDown={(e) => { if (e.key === 'Enter') commitEdit(); if (e.key === 'Escape') setEditing(null); }}
            style={{ width: '100%', fontSize: '1px', fontFamily: 'inherit', border: 'none', padding: 0, background: 'transparent' }}
          />
        </foreignObject>
      ) : (
        <text
          x={w / 2}
          y={isInitial ? h / 2 + 0.4 : titleHeight / 2 + 0.4}
          textAnchor="middle"
          className="state-title"
          onDoubleClick={onDoubleClick}
        >
          {state.title}
        </text>
      )}

      {/* Text lines */}
      {!isInitial && state.text.length > 0 && (
        <text
          x={0.3}
          y={titleHeight + 1.2}
          className="state-text"
          onDoubleClick={(e) => {
            e.stopPropagation();
            setEditing('text');
            setEditValue(state.text.join('\n'));
          }}
        >
          {state.text.map((line, i) => (
            <tspan key={i} x={0.3} dy={i === 0 ? 0 : 1.4}>
              {line}
            </tspan>
          ))}
        </text>
      )}

      {editing === 'text' && (
        <foreignObject x={0.2} y={titleHeight + 0.2} width={w - 0.4} height={h - titleHeight - 0.4}>
          <textarea
            autoFocus
            className="state-inline-edit"
            value={editValue}
            onChange={(e) => setEditValue(e.target.value)}
            onBlur={commitEdit}
            onKeyDown={(e) => { if (e.key === 'Escape') setEditing(null); }}
            style={{ width: '100%', height: '100%', fontSize: '1px', fontFamily: 'inherit', border: 'none', padding: 0, background: 'rgba(255,255,255,0.9)', resize: 'none' }}
          />
        </foreignObject>
      )}

      {/* Drag handle (invisible overlay) */}
      <rect
        width={w}
        height={isInitial ? h : titleHeight}
        fill="transparent"
        style={{ cursor: 'move' }}
        onMouseDown={onDragStart}
        onDoubleClick={onDoubleClick}
      />

      {/* Resize handle (bottom-right) */}
      {!isInitial && (
        <circle
          cx={w}
          cy={h}
          r={0.5}
          className="state-resize-handle"
          style={{ cursor: 'nwse-resize' }}
          onMouseDown={onResizeStart}
        />
      )}

      {/* Debug active border */}
      {isActive && (
        <rect
          width={w}
          height={h}
          rx={0.5}
          ry={0.5}
          fill="none"
          stroke="var(--debug-color, #ff6b00)"
          strokeWidth={0.25}
          strokeDasharray="0.6,0.3"
          pointerEvents="none"
        />
      )}
    </g>
  );
}
