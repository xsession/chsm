= Introduction

== What is CHSM?

CHSM (C Hierarchical State Machine) is an integrated toolchain for designing, generating, and executing UML-style hierarchical state machines on embedded targets.

The project consists of three layers:

+ *Visual Editor* — A browser-based GUI for drawing states and transitions.
+ *Code Generator* — Jinja2-based templates that produce compileable code for C, Python, JavaScript, Java, and VHDL.
+ *CRF Runtime Library* — A zero-malloc, ISR-safe active-object framework written in C for bare-metal and RTOS targets.

== Key Features

#table(
  columns: (1fr, 2fr),
  table.header([*Feature*], [*Description*]),
  [Visual Editor], [Browser-based drag-and-drop state machine designer with theme support.],
  [Multi-Language], [Generate C, Python, JavaScript, Java, and VHDL from a single model.],
  [Zero Malloc], [All queues and pools are statically pre-allocated — no `malloc` at runtime.],
  [ISR-Safe], [Event posting uses atomic compare-and-swap; safe to call from interrupts.],
  [Configurable], [Three flags (`CHSM_CFG_LITE`, `NO_FAULT_CNT`, `NO_DEBUG`) trade features for smaller footprint.],
  [Debug Channel], [TCP-based real-time state monitoring from the GUI editor.],
  [Module Wizard], [Cookiecutter scaffolding generates build-ready module directories.],
)

== Document Scope

This reference manual covers:

- System architecture and design principles.
- Complete C API reference for all runtime structs, functions, and macros.
- Code generation pipeline and template customisation.
- Build system and runtime configuration options.

For quick-start tutorials, see the HTML documentation built with Sphinx.
