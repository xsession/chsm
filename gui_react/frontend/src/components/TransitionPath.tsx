import { useCallback, useState } from 'react';
import { useStore } from '../store';
import type { ChsmTransition, ChsmConnector, ChsmState } from '../types';

interface Props {
  id: string;
  tr: ChsmTransition;
  connectors: Record<string, ChsmConnector>;
  states: Record<string, ChsmState>;
}

export default function TransitionPath({ id, tr, connectors, states }: Props) {
  const updateTransition = useStore((s) => s.updateTransition);
  const pushHistory = useStore((s) => s.pushHistory);
  const [editing, setEditing] = useState(false);
  const [editValue, setEditValue] = useState('');

  const startConn = connectors[tr.start];
  const endConn = connectors[tr.end];
  if (!startConn || !endConn) return null;

  const startState = states[startConn.parent];
  const endState = states[endConn.parent];
  if (!startState || !endState) return null;

  // Compute start/end points from state centers (simplified)
  const startPt = connPoint(startState, startConn);
  const endPt = connPoint(endState, endConn);

  // Build path from vertices or simple start→end
  const verts = tr.vertices && tr.vertices.length > 0 ? tr.vertices : [startPt, endPt];
  const allPts: [number, number][] = [startPt, ...verts.slice(1, -1), endPt];
  const d = allPts.map((p, i) => `${i === 0 ? 'M' : 'L'} ${p[0]} ${p[1]}`).join(' ');

  // Label position — midpoint of path
  const midIdx = Math.floor(allPts.length / 2);
  const lx = allPts.length >= 2
    ? (allPts[midIdx - 1][0] + allPts[midIdx][0]) / 2 + (tr.label_offset?.[0] ?? 0.5)
    : startPt[0];
  const ly = allPts.length >= 2
    ? (allPts[midIdx - 1][1] + allPts[midIdx][1]) / 2 + (tr.label_offset?.[1] ?? -0.4)
    : startPt[1];

  // Arrow angle at end
  const last = allPts[allPts.length - 1];
  const prev = allPts[allPts.length - 2] || last;
  const angle = Math.atan2(last[1] - prev[1], last[0] - prev[0]) * (180 / Math.PI);

  const commitLabel = useCallback(() => {
    updateTransition(id, { label: editValue });
    setEditing(false);
    pushHistory();
  }, [id, editValue, updateTransition, pushHistory]);

  return (
    <g className="transition-group">
      {/* Invisible thick hit area */}
      <path d={d} fill="none" stroke="transparent" strokeWidth={0.8} />

      {/* Visible line */}
      <path d={d} fill="none" className="transition-line" markerEnd="url(#arrowhead)" />

      {/* Label */}
      {editing ? (
        <foreignObject x={lx - 3} y={ly - 0.8} width={8} height={1.6}>
          <input
            autoFocus
            value={editValue}
            onChange={(e) => setEditValue(e.target.value)}
            onBlur={commitLabel}
            onKeyDown={(e) => { if (e.key === 'Enter') commitLabel(); if (e.key === 'Escape') setEditing(false); }}
            style={{ width: '100%', fontSize: '1px', fontFamily: 'inherit', border: 'none', padding: 0, background: 'rgba(255,255,255,0.9)' }}
          />
        </foreignObject>
      ) : (
        <text
          x={lx}
          y={ly}
          className="transition-label"
          onDoubleClick={(e) => {
            e.stopPropagation();
            setEditValue(tr.label);
            setEditing(true);
          }}
        >
          {tr.label}
        </text>
      )}
    </g>
  );
}

function connPoint(st: ChsmState, conn: ChsmConnector): [number, number] {
  const [x, y] = st.pos;
  const [w, h] = st.size;
  switch (conn.side) {
    case 'right':  return [x + w, y + h / 2];
    case 'left':   return [x, y + h / 2];
    case 'top':    return [x + w / 2, y];
    case 'bottom': return [x + w / 2, y + h];
    default:       return [x + w / 2, y + h / 2];
  }
}
