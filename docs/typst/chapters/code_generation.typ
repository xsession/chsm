= Code Generation

== Pipeline

The code generator transforms a visual model into compileable source files through three stages:

+ *JSON Model* — The `.chsm` file saved by the visual editor.
+ *Parser / AST* — `parser.py` walks the JSON, resolves parent chains, computes exit/entry paths, and builds a flat AST dictionary.
+ *Jinja2 Rendering* — Language-specific `.j2` templates consume the AST and produce output files.

== Supported Languages

#table(
  columns: (1fr, 2fr, 2fr),
  table.header([*Language*], [*Generator*], [*Template*]),
  [C], [`gui/c_gen/hsm/sm.py`], [`sm_temp.c.j2`, `sm_temp.h.j2`],
  [Python], [`gui/py_gen/render.py`], [`state_machine_template.py.j2`],
  [JavaScript], [`gui/js_gen/render.py`], [`state_machine_template.js.j2`],
  [Java], [`gui/java_gen/render.py`], [`state_machine_template.java.j2`],
  [VHDL], [`gui/vhdl_gen/render.py`], [`state_machine_template.vhdl.j2`],
)

== AST Structure

The parser produces a dictionary with the following top-level keys:

- *name* — Module name (string).
- *states* — List of state objects with `name`, `id`, `parent`, `entry`, `exit`, `children`.
- *transitions* — List with `source`, `target`, `signal`, `guard`, `actions`, plus pre-computed `exit_path` and `entry_path`.
- *signals* — Deduplicated list of signal names.

The `exit_path` and `entry_path` fields are the critical output of the parser: they contain the ordered list of user functions (exit bottom-up, entry top-down) that must execute during a transition. The runtime does not compute these at run time.

== C Generator Extras

The C generator produces additional files beyond the state machine skeleton:

- *Function stubs* (`sm_functions_temp.c.j2` / `.h.j2`) — Skeleton implementations of entry, exit, guard, and action functions. These are generated once and preserved across regeneration.
- *Unit test scaffold* (`unittest_temp.c.j2`) — A Unity test file with one test case per transition.

== Adding a Language

To add a new target language:

+ Create `gui/<lang>_gen/` directory.
+ Implement `render.py` with a `render(ast, output_path)` function.
+ Create a Jinja2 `.j2` template.
+ Register the language in the editor's Generate menu.

== CLI Usage

All generators can be invoked from the command line without the GUI:

```bash
python -m gui.c_gen.hsm.sm    model.json --output build/
python -m gui.py_gen.render    model.json
python -m gui.js_gen.render    model.json
python -m gui.java_gen.render  model.json
python -m gui.vhdl_gen.render  model.json
```
