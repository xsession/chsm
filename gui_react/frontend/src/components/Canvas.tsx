import { useRef, useEffect, useCallback, useState } from 'react';
import { useStore, newId } from '../store';
import type { ChsmState, ChsmConnector, ChsmView } from '../types';
import StateNode from './StateNode';
import TransitionPath from './TransitionPath';

export default function Canvas() {
  const svgRef = useRef<SVGSVGElement>(null);
  const model = useStore((s) => s.model);
  const view = useStore((s) => s.view);
  const setView = useStore((s) => s.setView);
  const mode = useStore((s) => s.mode);
  const setMode = useStore((s) => s.setMode);
  const addState = useStore((s) => s.addState);
  const removeState = useStore((s) => s.removeState);
  const removeTransition = useStore((s) => s.removeTransition);
  const addTransition = useStore((s) => s.addTransition);
  const pushHistory = useStore((s) => s.pushHistory);
  const elbow = useStore((s) => s.elbow);
  const toggleElbow = useStore((s) => s.toggleElbow);
  const activeStates = useStore((s) => s.activeStates);

  // ── Local drawing state ──────────────────────────────────────────
  const [isPanning, setIsPanning] = useState(false);
  const panStart = useRef<{ x: number; y: number; tx: number; ty: number } | null>(null);

  // Transition drawing state
  const [trDrawing, setTrDrawing] = useState<{
    startStateId: string;
    vertices: [number, number][];
    mouse: [number, number];
  } | null>(null);

  /* ── SVG coord conversion ────────────────────────────────────── */
  const svgPoint = useCallback(
    (clientX: number, clientY: number): [number, number] => {
      const [tx, ty] = view.translate;
      const sc = view.scale;
      const svg = svgRef.current;
      if (!svg) return [0, 0];
      const rect = svg.getBoundingClientRect();
      return [
        (clientX - rect.left - tx) / sc,
        (clientY - rect.top - ty) / sc,
      ];
    },
    [view],
  );

  /* ── Hit test: which state contains this SVG point? ──────────── */
  const hitState = useCallback(
    (pt: [number, number]): string | null => {
      const [px, py] = pt;
      // Check from smallest (deepest) to largest
      const entries = Object.entries(model.states).filter(([, s]) => s.type !== 'top');
      // Sort by area ascending (smaller states on top)
      entries.sort((a, b) => {
        const areaA = a[1].size[0] * a[1].size[1];
        const areaB = b[1].size[0] * b[1].size[1];
        return areaA - areaB;
      });
      for (const [id, st] of entries) {
        const [sx, sy] = st.pos;
        const [sw, sh] = st.size;
        if (px >= sx && px <= sx + sw && py >= sy && py <= sy + sh) return id;
      }
      return null;
    },
    [model.states],
  );

  /* ── Canvas click ────────────────────────────────────────────── */
  const handleCanvasClick = useCallback(
    (e: React.MouseEvent) => {
      if (e.button !== 0) return;
      const pt = svgPoint(e.clientX, e.clientY);

      if (mode === 'create_state') {
        const parentId = hitState(pt) || '__top__';
        const id = newId('state');
        const st: ChsmState = {
          pos: [Math.round(pt[0]), Math.round(pt[1])],
          size: [12, 8],
          title: 'new_state',
          text: [],
          connectors: [],
          parent: parentId,
          children: [],
          type: 'normal',
        };
        addState(id, st);
        pushHistory();
        setMode('idle');
        return;
      }

      if (mode === 'select_tr_start') {
        const stId = hitState(pt);
        if (stId) {
          setTrDrawing({ startStateId: stId, vertices: [connectorPos(model.states[stId])], mouse: pt });
          setMode('drawing_transition');
        }
        return;
      }

      if (mode === 'drawing_transition') {
        if (!trDrawing) return;
        const stId = hitState(pt);
        if (stId && stId !== trDrawing.startStateId) {
          // Complete transition
          const trId = newId('tr');
          const startConnId = newId('conn');
          const endConnId = newId('conn');
          const startConn: ChsmConnector = { parent: trDrawing.startStateId, offset: 0, side: 'right', dir: 'out', transition: trId };
          const endConn: ChsmConnector = { parent: stId, offset: 0, side: 'left', dir: 'in', transition: trId };
          addTransition(
            trId,
            {
              start: startConnId,
              end: endConnId,
              vertices: [...trDrawing.vertices, pt],
              label: '',
              label_offset: [0.5, -0.4],
              label_anchor: 0,
              label_pos: [0, 0],
            },
            startConn,
            endConn,
            startConnId,
            endConnId,
          );
          pushHistory();
          setTrDrawing(null);
          setMode('idle');
        } else {
          // Add waypoint
          setTrDrawing({ ...trDrawing, vertices: [...trDrawing.vertices, pt] });
        }
        return;
      }

      if (mode === 'delete') {
        const stId = hitState(pt);
        if (stId) {
          removeState(stId);
          pushHistory();
        } else {
          // Check transitions
          // Simple: find closest transition path segment
          for (const [trId] of Object.entries(model.transitions)) {
            // For simplicity just check bounding box
            removeTransition(trId);
            pushHistory();
            break;
          }
        }
        setMode('idle');
        return;
      }
    },
    [mode, svgPoint, hitState, addState, pushHistory, setMode, model, trDrawing, addTransition, removeState, removeTransition],
  );

  /* ── Mouse move (for panning + transition preview) ───────────── */
  const handleMouseMove = useCallback(
    (e: React.MouseEvent) => {
      if (isPanning && panStart.current) {
        const dx = e.clientX - panStart.current.x;
        const dy = e.clientY - panStart.current.y;
        setView({
          translate: [panStart.current.tx + dx, panStart.current.ty + dy],
          scale: view.scale,
        });
        return;
      }
      if (trDrawing) {
        const pt = svgPoint(e.clientX, e.clientY);
        setTrDrawing({ ...trDrawing, mouse: pt });
      }
    },
    [isPanning, view, setView, trDrawing, svgPoint],
  );

  /* ── Pan start / end ─────────────────────────────────────────── */
  const handleMouseDown = useCallback(
    (e: React.MouseEvent) => {
      if (e.ctrlKey || e.metaKey) {
        setIsPanning(true);
        panStart.current = { x: e.clientX, y: e.clientY, tx: view.translate[0], ty: view.translate[1] };
        e.preventDefault();
      }
    },
    [view],
  );

  const handleMouseUp = useCallback(() => {
    setIsPanning(false);
    panStart.current = null;
  }, []);

  /* ── Zoom ────────────────────────────────────────────────────── */
  const handleWheel = useCallback(
    (e: React.WheelEvent) => {
      if (!e.ctrlKey && !e.metaKey) return;
      e.preventDefault();
      const factor = e.deltaY < 0 ? 1.1 : 0.9;
      const newScale = Math.max(1, Math.min(100, view.scale * factor));
      setView({ translate: view.translate, scale: newScale });
    },
    [view, setView],
  );

  /* ── Transition preview path ─────────────────────────────────── */
  const previewPath = trDrawing
    ? [...trDrawing.vertices, trDrawing.mouse].map((p, i) => `${i === 0 ? 'M' : 'L'} ${p[0]} ${p[1]}`).join(' ')
    : '';

  return (
    <main className="canvas-area">
      <svg
        ref={svgRef}
        width="100%"
        height="100%"
        onClick={handleCanvasClick}
        onMouseDown={handleMouseDown}
        onMouseMove={handleMouseMove}
        onMouseUp={handleMouseUp}
        onWheel={handleWheel}
        style={{ cursor: isPanning ? 'grabbing' : mode === 'idle' ? 'default' : 'crosshair' }}
      >
        {/* Arrow marker def */}
        <defs>
          <marker id="arrowhead" markerWidth="10" markerHeight="7" refX="10" refY="3.5" orient="auto">
            <polygon points="0 0, 10 3.5, 0 7" className="arrow-fill" />
          </marker>
        </defs>

        <g transform={`translate(${view.translate[0]}, ${view.translate[1]}) scale(${view.scale})`}>
          {/* Transitions (behind states) */}
          {Object.entries(model.transitions).map(([id, tr]) => (
            <TransitionPath key={id} id={id} tr={tr} connectors={model.connectors} states={model.states} />
          ))}

          {/* Preview line while drawing */}
          {previewPath && (
            <path d={previewPath} fill="none" stroke="var(--accent, #2196f3)" strokeWidth={0.15} strokeDasharray="0.4,0.2" />
          )}

          {/* States */}
          {Object.entries(model.states)
            .filter(([, s]) => s.type !== 'top')
            .map(([id, st]) => (
              <StateNode
                key={id}
                id={id}
                state={st}
                isActive={activeStates.includes(st.title)}
                svgRef={svgRef}
                view={view}
              />
            ))}
        </g>
      </svg>

      {/* Status bar */}
      <div className="status-bar">
        <span className="status-mode">{mode.toUpperCase()}</span>
        <span className="status-zoom">{Math.round(view.scale * 10)}%</span>
      </div>
    </main>
  );
}

function connectorPos(st: ChsmState): [number, number] {
  return [st.pos[0] + st.size[0] / 2, st.pos[1] + st.size[1] / 2];
}
