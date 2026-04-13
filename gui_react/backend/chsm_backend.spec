# -*- mode: python ; coding: utf-8 -*-
"""
PyInstaller spec for CHSM backend.

Run from gui_react/:
    pyinstaller --noconfirm backend/chsm_backend.spec

Output goes to gui_react/dist_backend/
"""
import os, sys
from pathlib import Path
from PyInstaller.building.build_main import Analysis, PYZ, EXE, COLLECT

spec_dir   = os.path.dirname(os.path.abspath(SPECPATH)) if 'SPECPATH' in dir() else os.path.dirname(os.path.abspath(__file__))
gui_react  = Path(spec_dir).parent.resolve() if Path(spec_dir).name == 'backend' else Path(spec_dir).resolve()
backend    = gui_react / 'backend'
gui        = gui_react.parent / 'gui'
frontend   = gui_react / 'frontend'

# Collect all c_gen/, templates, and the gui helper modules that server.py imports
datas = [
    # React build (served by FastAPI static mount)
    (str(frontend / 'dist'),       'frontend/dist'),
    # c_gen package (generators, templates, Jinja2 templates)
    (str(gui / 'c_gen'),           'c_gen'),
    # gui web assets needed by _save_html()
    (str(gui / 'web' / 'drawing.css'),         'web'),
    (str(gui / 'web' / 'themes'),              'web/themes'),
]

# Add c_gen/templates directory explicitly
templates_dir = gui / 'c_gen' / 'templates'
if templates_dir.exists():
    datas.append((str(templates_dir), 'c_gen/templates'))

hidden_imports = [
    'uvicorn',
    'uvicorn.logging',
    'uvicorn.loops',
    'uvicorn.loops.auto',
    'uvicorn.protocols',
    'uvicorn.protocols.http',
    'uvicorn.protocols.http.auto',
    'uvicorn.protocols.websockets',
    'uvicorn.protocols.websockets.auto',
    'uvicorn.lifespan',
    'uvicorn.lifespan.on',
    'fastapi',
    'starlette',
    'starlette.routing',
    'starlette.middleware',
    'starlette.middleware.cors',
    'anyio',
    'anyio._backends',
    'anyio._backends._asyncio',
    'jinja2',
    'markupsafe',
    'websockets',
]

a = Analysis(
    [str(backend / 'server_entry.py')],
    pathex=[str(gui), str(backend)],
    binaries=[],
    datas=datas,
    hiddenimports=hidden_imports,
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=['tkinter'],  # not needed — Electron provides native dialogs
    noarchive=False,
)

pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    [],
    exclude_binaries=True,
    name='chsm_backend',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    console=True,        # keep console for logging in dev; hide with --noconsole flag
)

coll = COLLECT(
    exe,
    a.binaries,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='chsm_backend',
)
