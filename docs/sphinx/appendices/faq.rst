.. _appendix_faq:

==========================
FAQ
==========================

General
========

**What is CHSM?**
   CHSM (C Hierarchical State Machine) is a visual editor and code
   generation framework for designing UML-style hierarchical state
   machines and generating runtime code for embedded systems.

**Which languages are supported?**
   C, Python, JavaScript, Java, and VHDL.

**Is the generated C code suitable for safety-critical systems?**
   The runtime uses no dynamic memory, no recursion beyond bounded
   nesting, and deterministic execution.  It can serve as a foundation
   for safety-relevant code, but formal qualification (e.g. DO-178C,
   IEC 61508) requires additional verification steps.

Runtime
========

**How deep can states be nested?**
   Up to ``CHSM_MAX_NESTING_LEVEL`` (default 8).  Increase the constant
   in ``chsm.h`` if deeper hierarchies are required.

**What happens when a queue overflows?**
   ``cqueue_put`` returns ``-1`` and increments ``fault_cnt`` (unless
   disabled).  The event is **not** silently dropped.

**Can I use CHSM with an RTOS?**
   Yes.  Call ``crf_step()`` from a dedicated task.  Queue put
   operations are ISR-safe.  Protect ``crf_step`` with a mutex if
   multiple tasks consume events.

**How do I reduce RAM usage?**
   Enable ``CHSM_CFG_LITE``, ``CHSM_CFG_NO_FAULT_CNT``, and
   ``CHSM_CFG_NO_DEBUG``.  See :ref:`config_runtime_tuning`.

Code Generation
================

**Can I regenerate code without losing my function implementations?**
   Yes.  The generator outputs the state-machine skeleton
   (``<module>.c/.h``) and a separate functions file
   (``<module>_functions.c/.h``).  Only the skeleton is overwritten;
   your function bodies are preserved.

**How do I add a new target language?**
   1. Create a ``gui/<lang>_gen/`` directory.
   2. Write a ``render.py`` with a ``render(ast)`` function.
   3. Add a Jinja2 ``.j2`` template.
   4. Register the language in the editor's Generate menu.

GUI
====

**The editor window doesn't open.**
   Ensure Chromium or Chrome is installed.  Eel requires a Chromium-based
   browser.  Alternatively, set ``eel.start(..., mode='default')`` to
   use the system default browser.

**Can I run the editor headless for CI?**
   Use the code-generation modules directly from Python without the GUI:

   .. code-block:: bash

      python -m gui.c_gen.hsm.sm model.json --output build/gen/
