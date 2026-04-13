.. _architecture_overview:

======================
Architecture Overview
======================

CHSM is organised into three layers that can be used independently.

.. mermaid::

   graph TD
     A[GUI Editor<br/>Python + JS] --> B[Code Generator<br/>AST → C / Py / JS / Java / VHDL]
     B --> C[CRF Runtime<br/>C11 library]
     C --> D[Target Hardware<br/>ARM Cortex-M / POSIX]

Layers
======

GUI Editor
----------

A browser-based drawing tool served by a Python Eel backend.
State machines are stored as JSON embedded in self-contained HTML files,
making them viewable in any browser without the editor.

* **Frontend:** Vanilla JavaScript (no framework), SVG canvas.
* **Backend:** ``chsm_backend.py`` — Eel web-socket bridge, file I/O,
  code generation trigger, debug server.
* **Themes:** Solarized Light, Solarized Dark, Default.

Code Generator
--------------

Transforms the model JSON into compilable source code.

.. list-table::
   :widths: 15 20 65
   :header-rows: 1

   * - Language
     - Module
     - Output
   * - C
     - ``gui/c_gen/hsm/sm.py``
     - AST-based ``.c`` + ``_functions.h`` via ``ast.py``
   * - Python
     - ``gui/py_gen/render.py``
     - Jinja2 template → ``.py``
   * - JavaScript
     - ``gui/js_gen/render.py``
     - Jinja2 template → ``.js``
   * - Java
     - ``gui/java_gen/render.py``
     - Jinja2 template → ``.java``
   * - VHDL
     - ``gui/vhdl_gen/render.py``
     - Jinja2 template → ``.vhd``

The C generator (``sm.py``) constructs a full abstract syntax tree and emits
C source through ``__str__`` methods on the AST nodes.  All other generators
use the shared Jinja state machine class (``sm_jinja.py``) with
language-specific templates.

CRF Runtime Library
-------------------

A self-contained C11 library at ``languages/c/c_rf/``.

.. list-table::
   :widths: 20 80
   :header-rows: 1

   * - File
     - Purpose
   * - ``chsm.c / .h``
     - Hierarchical state machine core (ctor, init, dispatch, defer, recall)
   * - ``cevent.h``
     - 4-byte base event (signal + GC bit-field)
   * - ``cqueue.c / .h``
     - Wait-free MPSC event queue (atomic head/tail)
   * - ``cpool.c / .h``
     - Lock-free O(1) memory pool (CAS-based allocation)
   * - ``crf.c / .h``
     - Active object framework (new, post, step, gc)
   * - ``chsm_cfg.h``
     - Compile-time footprint configuration

Directory Layout
=================

::

   chsm/
   ├── gui/                    Editor + code generators
   │   ├── chsm_backend.py    Eel backend
   │   ├── c_gen/              C code generator (AST-based)
   │   ├── py_gen/             Python generator (Jinja2)
   │   ├── js_gen/             JavaScript generator (Jinja2)
   │   ├── java_gen/           Java generator (Jinja2)
   │   ├── vhdl_gen/           VHDL generator (Jinja2)
   │   ├── module_generator/   New-module wizard (cookiecutter)
   │   └── web/                HTML/CSS/JS frontend
   ├── languages/
   │   └── c/
   │       ├── c_rf/           CRF runtime library
   │       ├── modules/        Protocol & device driver modules
   │       └── unity/          Unity test framework (submodule)
   └── docs/                   This documentation
