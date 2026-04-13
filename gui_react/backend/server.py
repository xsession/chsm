"""
CHSM React Backend — FastAPI + WebSocket server.

Usage:
    uvicorn server:app --reload --port 8000

Provides:
    REST  /api/...           – file I/O, code generation, module creation
    WS    /ws                – real-time events (debug channel, save-status)
    Static /                 – serves React build (production)
"""
import json
import logging
import os
import re
import socket
import select
import sys
import threading
from pathlib import Path
from typing import Optional

from fastapi import FastAPI, WebSocket, WebSocketDisconnect, UploadFile, File
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse, JSONResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel

# ── Resolve project paths ──────────────────────────────────────────────
BACKEND_DIR = Path(__file__).parent.resolve()

# When running from a PyInstaller bundle, resources are in sys._MEIPASS
_MEIPASS = getattr(sys, '_MEIPASS', None)
if _MEIPASS:
    _bundle = Path(_MEIPASS)
    GUI_DIR = _bundle  # c_gen/ etc. are packed relative to bundle root
    C_GEN_DIR = _bundle / "c_gen"
    TEMPLATE_DIR = C_GEN_DIR / "templates"
    WEB_DIR = _bundle / "web"
    REACT_DIST_OVERRIDE = _bundle / "frontend" / "dist"
else:
    GUI_DIR = (BACKEND_DIR / ".." / ".." / "gui").resolve()
    C_GEN_DIR = GUI_DIR / "c_gen"
    TEMPLATE_DIR = C_GEN_DIR / "templates"
    WEB_DIR = GUI_DIR / "web"
    REACT_DIST_OVERRIDE = None

# Add gui/ to sys.path so existing generators import cleanly
sys.path.insert(0, str(GUI_DIR))

from c_gen import StateMachine
try:
    import module_generator
except ImportError:
    module_generator = None

# ── Logging ────────────────────────────────────────────────────────────
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s %(filename)-20s:%(lineno)-4s %(message)s",
)
log = logging.getLogger(__name__)

# ── FastAPI app ────────────────────────────────────────────────────────
app = FastAPI(title="CHSM Backend", version="1.0.0")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173", "http://localhost:3000", "http://127.0.0.1:5173"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

TOP_STATE_RE = re.compile(
    r'chsm_result_ten\s+(?P<top_func>\w+)\(chsm_tst\s+\*self,\s+const\s+cevent_tst\s+\*e_pst\)\s*;'
)


# ═══════════════════════════════════════════════════════════════════════
#  WebSocket hub — broadcasts to all connected React clients
# ═══════════════════════════════════════════════════════════════════════
class WsHub:
    def __init__(self):
        self._clients: list[WebSocket] = []

    async def connect(self, ws: WebSocket):
        await ws.accept()
        self._clients.append(ws)
        log.info("WS client connected (%d total)", len(self._clients))

    def disconnect(self, ws: WebSocket):
        self._clients.remove(ws)
        log.info("WS client disconnected (%d total)", len(self._clients))

    async def broadcast(self, event: str, data: dict):
        msg = json.dumps({"event": event, "data": data})
        dead = []
        for ws in self._clients:
            try:
                await ws.send_text(msg)
            except Exception:
                dead.append(ws)
        for ws in dead:
            self._clients.remove(ws)

    def broadcast_sync(self, event: str, data: dict):
        """Thread-safe broadcast (used from debug server thread)."""
        import asyncio
        msg = json.dumps({"event": event, "data": data})
        for ws in list(self._clients):
            try:
                asyncio.run_coroutine_threadsafe(
                    ws.send_text(msg),
                    _loop,
                )
            except Exception:
                pass


hub = WsHub()
_loop = None  # set on startup


@app.on_event("startup")
async def _capture_loop():
    import asyncio
    global _loop
    _loop = asyncio.get_running_loop()


@app.websocket("/ws")
async def websocket_endpoint(ws: WebSocket):
    await hub.connect(ws)
    try:
        while True:
            data = await ws.receive_text()
            msg = json.loads(data)
            cmd = msg.get("cmd")
            if cmd == "debug_start":
                port = int(msg.get("port", 9999))
                debug_server.start(port)
            elif cmd == "debug_stop":
                debug_server.stop()
            elif cmd == "debug_send":
                debug_server.send_to_target(msg.get("payload", {}))
    except WebSocketDisconnect:
        hub.disconnect(ws)


# ═══════════════════════════════════════════════════════════════════════
#  Debug TCP server — identical protocol to Eel version
# ═══════════════════════════════════════════════════════════════════════
class DebugServer:
    def __init__(self):
        self._server_socket: Optional[socket.socket] = None
        self._client: Optional[socket.socket] = None
        self._thread: Optional[threading.Thread] = None
        self._running = False
        self.port = 0
        self.log: list[dict] = []
        self.MAX_LOG = 200

    @property
    def connected(self):
        return self._client is not None

    def start(self, port: int):
        if self._running:
            self.stop()
        self.port = port
        self._running = True
        self._thread = threading.Thread(target=self._serve, daemon=True)
        self._thread.start()
        log.info("Debug server started on port %d", self.port)

    def stop(self):
        self._running = False
        for s in (self._client, self._server_socket):
            if s:
                try:
                    s.close()
                except Exception:
                    pass
        self._client = None
        self._server_socket = None
        hub.broadcast_sync("DEBUG_STATUS", {"connected": False, "listening": False})
        log.info("Debug server stopped")

    def _serve(self):
        try:
            self._server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self._server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self._server_socket.bind(("127.0.0.1", self.port))
            self._server_socket.listen(1)
            self._server_socket.settimeout(1.0)
        except OSError as e:
            log.error("Debug server bind failed: %s", e)
            hub.broadcast_sync("DEBUG_STATUS", {"connected": False, "error": str(e)})
            self._running = False
            return

        hub.broadcast_sync("DEBUG_STATUS", {"connected": False, "listening": True, "port": self.port})

        while self._running:
            try:
                ready, _, _ = select.select([self._server_socket], [], [], 1.0)
                if not ready:
                    continue
                client, addr = self._server_socket.accept()
            except (OSError, socket.timeout):
                continue

            self._client = client
            self._client.settimeout(1.0)
            log.info("Debug client connected from %s", addr)
            hub.broadcast_sync("DEBUG_STATUS", {"connected": True, "port": self.port})

            buf = ""
            while self._running and self._client:
                try:
                    data = self._client.recv(4096)
                    if not data:
                        break
                    buf += data.decode("utf-8", errors="replace")
                    while "\n" in buf:
                        line, buf = buf.split("\n", 1)
                        line = line.strip()
                        if not line:
                            continue
                        try:
                            msg = json.loads(line)
                            self._handle_message(msg)
                        except json.JSONDecodeError as e:
                            log.warning("Debug bad JSON: %s", e)
                except socket.timeout:
                    continue
                except (ConnectionResetError, OSError):
                    break

            self._client = None
            log.info("Debug client disconnected")
            hub.broadcast_sync("DEBUG_STATUS", {"connected": False, "listening": True, "port": self.port})
            hub.broadcast_sync("DEBUG_UPDATE", {"type": "reset"})

    def _handle_message(self, msg: dict):
        if msg.get("type") == "event":
            if len(self.log) >= self.MAX_LOG:
                self.log.pop(0)
            self.log.append(msg)
        hub.broadcast_sync("DEBUG_UPDATE", msg)

    def send_to_target(self, msg: dict):
        if self._client:
            try:
                self._client.sendall((json.dumps(msg) + "\n").encode("utf-8"))
            except (OSError, BrokenPipeError) as e:
                log.warning("Debug send failed: %s", e)


debug_server = DebugServer()


# ═══════════════════════════════════════════════════════════════════════
#  REST API
# ═══════════════════════════════════════════════════════════════════════

# ── File I/O ───────────────────────────────────────────────────────────

class SaveRequest(BaseModel):
    filepath: str
    model: dict
    svg: str = ""


class OpenResponse(BaseModel):
    model: dict
    filename: str
    filepath: str


def _load_default_model() -> dict:
    with open(TEMPLATE_DIR / "model.json") as f:
        return json.load(f)


def _extract_model_from_html(html_path: Path) -> Optional[dict]:
    if not html_path.exists():
        return None
    content = html_path.read_text(encoding="utf-8", errors="replace")
    m = re.search(r"<pre id='chsm-json-data'>\n(?P<json>.+)</pre>", content, re.DOTALL)
    if not m:
        return None
    try:
        return json.loads(m.group("json"))
    except json.JSONDecodeError:
        return None


def _save_html(html_path: Path, svg: str, json_data: str):
    template_html = TEMPLATE_DIR / "template.html"
    drawing_css = WEB_DIR / "drawing.css"
    drawing_js = TEMPLATE_DIR / "wheel.js"
    theme_css = WEB_DIR / "themes" / "default.css"

    for p in (template_html, drawing_css, drawing_js, theme_css):
        if not p.exists():
            raise FileNotFoundError(f"Missing: {p}")

    template = template_html.read_text()
    css = drawing_css.read_text()
    js = drawing_js.read_text()
    theme = theme_css.read_text()
    output = template.format(style=css, theme_style=theme, drawing=svg, json_data=json_data, script=js)
    html_path.parent.mkdir(parents=True, exist_ok=True)
    html_path.write_text(output, encoding="utf-8")


def _find_settings(start_dir: Path, stem: Optional[str] = None) -> dict:
    """Walk up from start_dir looking for .chsm/settings.json or .chsm/<stem>.json."""
    for parent in [start_dir] + list(start_dir.parents):
        if stem:
            p = parent / ".chsm" / f"{stem}.json"
            if p.exists():
                with open(p) as f:
                    return json.load(f)
        p = parent / ".chsm" / "settings.json"
        if p.exists():
            with open(p) as f:
                return json.load(f)
    return {}


@app.post("/api/save")
async def api_save(req: SaveRequest):
    try:
        filepath = Path(req.filepath)
        if filepath.suffix == ".html":
            _save_html(filepath, req.svg, json.dumps(req.model))
        else:
            filepath.parent.mkdir(parents=True, exist_ok=True)
            filepath.write_text(json.dumps(req.model, indent=2), encoding="utf-8")
        return {"ok": True, "filepath": str(filepath), "filename": filepath.name}
    except Exception as e:
        log.exception("Save failed")
        return JSONResponse({"ok": False, "error": str(e)}, status_code=500)


@app.post("/api/open")
async def api_open(filepath: str = ""):
    """Open a .h or .html file and return the model JSON."""
    if not filepath:
        return JSONResponse({"ok": False, "error": "No filepath provided"}, status_code=400)

    p = Path(filepath)
    if not p.exists():
        return JSONResponse({"ok": False, "error": f"File not found: {filepath}"}, status_code=404)

    if p.suffix == ".h":
        # Derive paths the same way as the Eel backend
        default_cfg = json.loads((TEMPLATE_DIR / "settings.json").read_text())
        user_cfg = _find_settings(p.parent, p.stem)
        file_cfg = user_cfg.get(p.name, {})
        src_dir = user_cfg.get("src_dir", default_cfg.get("src_dir", "../src"))
        doc_dir = user_cfg.get("doc_dir", default_cfg.get("doc_dir", "../doc"))

        html_name = file_cfg.get("machine_html", p.stem + ".html")
        html_path = (p.parent / doc_dir / html_name).resolve()

        model = _extract_model_from_html(html_path)
        if model is None:
            model = _load_default_model()
            # Try to extract top state name from .h
            h_content = p.read_text(errors="replace")
            m = TOP_STATE_RE.search(h_content)
            if m:
                model["states"]["__top__"]["title"] = m.group("top_func")
            else:
                model["states"]["__top__"]["title"] = f"{p.stem}__top__"

        return {"ok": True, "model": model, "filename": p.name, "filepath": str(p),
                "html_path": str(html_path), "src_dir": src_dir, "doc_dir": doc_dir,
                "file_cfg": file_cfg}

    elif p.suffix == ".html":
        model = _extract_model_from_html(p)
        if model is None:
            return JSONResponse({"ok": False, "error": "No JSON data found in HTML"}, status_code=400)
        return {"ok": True, "model": model, "filename": p.name, "filepath": str(p)}

    return JSONResponse({"ok": False, "error": "Unsupported file type"}, status_code=400)


# ── Code generation ────────────────────────────────────────────────────

class CodeGenRequest(BaseModel):
    model: dict
    h_filepath: str
    file_cfg: dict = {}


@app.post("/api/codegen")
async def api_codegen(req: CodeGenRequest):
    try:
        h_path = Path(req.h_filepath)
        default_cfg = json.loads((TEMPLATE_DIR / "settings.json").read_text())
        user_cfg = _find_settings(h_path.parent, h_path.stem)
        file_cfg = req.file_cfg or user_cfg.get(h_path.name, {})
        c_templates = user_cfg.get("templates", default_cfg.get("templates"))
        src_dir = user_cfg.get("src_dir", default_cfg.get("src_dir", "../src"))

        func_h_name = file_cfg.get("func_h", h_path.stem + "_functions.h")
        func_h_path = (h_path.parent / func_h_name).resolve()
        c_name = file_cfg.get("machine_c", h_path.stem + ".c")
        c_path = (h_path.parent / src_dir / c_name).resolve()

        sm = StateMachine(req.model, h_path, func_h_path.name, c_templates, file_cfg)

        c_path.parent.mkdir(parents=True, exist_ok=True)
        c_path.write_text(str(sm.ast), encoding="utf-8")
        func_h_path.write_text(str(sm.h_ast), encoding="utf-8")

        log.info("Code generated: %s, %s", c_path.name, func_h_path.name)
        return {"ok": True, "c_file": str(c_path), "h_file": str(func_h_path)}
    except Exception as e:
        log.exception("Code generation failed")
        return JSONResponse({"ok": False, "error": str(e)}, status_code=500)


# ── Module generator ───────────────────────────────────────────────────

class ModuleGenRequest(BaseModel):
    name: str
    version: str = ""
    description: str = ""
    location: str = ""
    linked_libs: str = ""
    package_name: str = ""
    comm_periph: str = "none"
    device_address: str = ""
    author_nick_name: str = ""
    author_full_name: str = ""
    licence: str = "MIT"


@app.post("/api/new-module")
async def api_new_module(req: ModuleGenRequest):
    if module_generator is None:
        return JSONResponse({"ok": False, "error": "module_generator not available"}, status_code=500)
    try:
        module_generator.generate_module(
            req.name, req.version, req.description, req.location,
            req.linked_libs, req.package_name, req.comm_periph,
            req.device_address, req.author_nick_name,
            req.author_full_name, req.licence,
        )
        return {"ok": True}
    except Exception as e:
        log.exception("Module generation failed")
        return JSONResponse({"ok": False, "error": str(e)}, status_code=500)


# ── File picker (native dialog via tkinter in a thread) ────────────────

@app.get("/api/pick-file")
async def api_pick_file(mode: str = "open", filetypes: str = ".h,.html"):
    """Open a native file picker dialog. Runs tkinter in a thread."""
    import asyncio

    def _pick():
        import tkinter as tk
        from tkinter.filedialog import askopenfilename, asksaveasfilename
        root = tk.Tk()
        root.attributes("-topmost", True)
        root.withdraw()
        ft = [(f"File (*{e})", f"*{e}") for e in filetypes.split(",")]
        if mode == "save":
            result = asksaveasfilename(title="Save state machine", filetypes=ft)
        else:
            result = askopenfilename(title="Open file", filetypes=ft)
        root.destroy()
        return result

    loop = asyncio.get_running_loop()
    path = await loop.run_in_executor(None, _pick)
    if path:
        return {"ok": True, "filepath": path}
    return {"ok": False, "cancelled": True}


# ── Health ─────────────────────────────────────────────────────────────

@app.get("/api/health")
async def health():
    return {"status": "ok", "debug_connected": debug_server.connected}


# ── Serve React build in production ───────────────────────────────────
_react_dist = REACT_DIST_OVERRIDE or (BACKEND_DIR / ".." / "frontend" / "dist")
if _react_dist.exists():
    app.mount("/", StaticFiles(directory=str(_react_dist.resolve()), html=True))
