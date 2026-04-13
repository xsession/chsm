.. _gui_code_generation:

==========================
Code Generation Pipeline
==========================

The code generator transforms the visual model into compileable source
files for five target languages.

Pipeline Overview
==================

.. mermaid::

   flowchart LR
     JSON["Model JSON"] --> Parser["parser.py"]
     Parser --> AST["Internal AST"]
     AST --> Jinja["Jinja2 Templates"]
     Jinja --> C["C output"]
     Jinja --> Py["Python output"]
     Jinja --> JS["JavaScript output"]
     Jinja --> Java["Java output"]
     Jinja --> VHDL["VHDL output"]

1. **Model JSON** — The ``.chsm`` file saved by the editor.
2. **parser.py** — Walks the JSON, resolves parent chains, computes
   exit/entry paths, and builds a flat AST dictionary.
3. **Jinja2 Templates** — Language-specific ``.j2`` templates render
   the AST into source code.

Supported Languages
====================

.. list-table::
   :widths: 15 30 30 25
   :header-rows: 1

   * - Language
     - Generator module
     - Template
     - Output
   * - C
     - ``gui/c_gen/hsm/sm.py``
     - ``sm_temp.c.j2``, ``sm_temp.h.j2``
     - ``<module>.c``, ``<module>.h``
   * - Python
     - ``gui/py_gen/render.py``
     - ``state_machine_template.py.j2``
     - ``<module>.py``
   * - JavaScript
     - ``gui/js_gen/render.py``
     - ``state_machine_template.js.j2``
     - ``<module>.js``
   * - Java
     - ``gui/java_gen/render.py``
     - ``state_machine_template.java.j2``
     - ``<module>.java``
   * - VHDL
     - ``gui/vhdl_gen/render.py``
     - ``state_machine_template.vhdl.j2``
     - ``<module>.vhdl``

AST Structure
==============

The parser produces a dictionary consumed by every template:

.. code-block:: python

   {
       "name": "my_hsm",
       "states": [
           {"name": "idle", "id": 0, "parent": None,
            "entry": "idle_entry", "exit": "idle_exit",
            "children": [1, 2]},
           ...
       ],
       "transitions": [
           {"source": 0, "target": 1,
            "signal": "SIG_START", "guard": None,
            "actions": ["start_motor"],
            "exit_path": ["idle_exit"],
            "entry_path": ["running_entry"]},
           ...
       ],
       "signals": ["SIG_START", "SIG_STOP", ...],
   }

Key computed fields:

* **exit_path** / **entry_path** — Pre-calculated lists of user
  functions to call during a transition (exits bottom-up, entries
  top-down).

C Code Generator Details
==========================

The C generator (``gui/c_gen/hsm/sm.py``) produces two additional
artefacts per module on top of the Jinja render:

* **Function stubs** — ``sm_functions_temp.c.j2`` / ``.h.j2`` generate
  skeleton entry/exit/guard functions that the developer fills in.
* **Unit test scaffold** — ``unittest_temp.c.j2`` generates a Unity
  test file with one test case per transition.

Invoking from CLI
==================

.. code-block:: bash

   python -m gui.c_gen.hsm.sm  path/to/model.json  --output build/

   python -m gui.py_gen.render  path/to/model.json
   python -m gui.js_gen.render  path/to/model.json
   python -m gui.java_gen.render path/to/model.json
   python -m gui.vhdl_gen.render path/to/model.json
