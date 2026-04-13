= Architecture

== Layer Model

The CHSM system is organised into three layers. Each layer communicates strictly downward — the runtime has no dependency on the GUI.

#table(
  columns: (1fr, 3fr),
  table.header([*Layer*], [*Responsibility*]),
  [GUI Editor], [Visual state/transition editing, JSON model persistence, theme management, debug panel.],
  [Code Generator], [Parse JSON model → build AST → render Jinja2 templates → output source files.],
  [CRF Runtime], [Run-to-completion event loop, static memory pools, atomic queues, garbage collection.],
)

== Execution Model

The CRF runtime uses a *run-to-completion* (RTC) execution model:

+ `crf_step()` iterates over all registered state machines.
+ For each HSM, it dequeues one event from the event queue.
+ The event is dispatched to the current state handler.
+ If the handler returns `C_RES_PARENT`, the event propagates up the hierarchy.
+ If the handler returns `C_RES_TRANS`, exit and entry functions are called in order.
+ After dispatch, `crf_gc()` decrements the reference count and frees the event if it reaches zero.

No preemption occurs within a single dispatch cycle. This guarantees that state handlers execute atomically with respect to other events.

== Struct Relationships

The core data structures form a clear ownership hierarchy:

- *`crf_tst`* owns the array of state machines and memory pools.
- *`chsm_tst`* embeds two `cqueue_tst` instances (event queue and defer queue).
- *`cpool_tst`* manages a contiguous buffer of fixed-size event blocks.
- *`cevent_tst`* is the base event type with signal and GC metadata.
- All derived events embed `cevent_tst` as their first member.

== Thread Safety Model

#table(
  columns: (1fr, 1fr, 2fr),
  table.header([*Operation*], [*Safety*], [*Notes*]),
  [`cqueue_put`], [ISR-safe], [Atomic CAS on head; multiple producers allowed.],
  [`cqueue_get`], [Main only], [Single consumer assumed.],
  [`cpool_new`], [ISR-safe], [CAS-based free-list pop.],
  [`cpool_gc`], [ISR-safe], [CAS-based free-list push.],
  [`crf_step`], [Main only], [Iterates HSMs sequentially in a single thread.],
)

== Directory Layout

```
chsm/
├── gui/                    # Visual editor + code generators
│   ├── chsm_backend.py    # Eel backend + debug server
│   ├── c_gen/              # C code generator
│   ├── py_gen/             # Python code generator
│   ├── js_gen/             # JavaScript code generator
│   ├── java_gen/           # Java code generator
│   ├── vhdl_gen/           # VHDL code generator
│   └── web/                # Frontend HTML/CSS/JS
├── languages/
│   └── c/                  # C runtime + modules
│       ├── c_rf/           # CRF library (cevent, cqueue, cpool, crf)
│       ├── modules/        # HSM modules (chsm core + applications)
│       ├── cmake_utils/    # CMake helper macros
│       └── unity/          # Unity test framework
└── docs/
    ├── sphinx/             # Sphinx documentation (HTML/PDF)
    └── typst/              # Typst documentation (PDF)
```
