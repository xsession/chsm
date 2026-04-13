.. _index:

==============================================
CHSM — Hierarchical State Machine Framework
==============================================

.. image:: https://github.com/xsession/chsm/workflows/chsm_crf_ci/badge.svg
   :target: https://github.com/xsession/chsm/actions
   :alt: CI Status

.. image:: https://github.com/xsession/chsm/workflows/chsm_modules_ci/badge.svg
   :target: https://github.com/xsession/chsm/actions
   :alt: Modules CI Status

**CHSM** is a complete toolkit for designing, editing and executing
UML-compliant hierarchical state machines targeting embedded C systems.
It consists of a graphical editor, a multi-language code generator and
the **CRF** (C Reactive Framework) runtime library.

Key Features
============

.. grid:: 2
   :gutter: 3

   .. grid-item-card:: Visual State Machine Editor
      :margin: 0

      Draw, annotate and simulate state machines in a browser-based GUI.
      Export as self-contained HTML or generate production code.

   .. grid-item-card:: Multi-Language Code Generation
      :margin: 0

      Generate C, Python, JavaScript, Java and VHDL state machine
      implementations from a single model.

   .. grid-item-card:: CRF Runtime Library
      :margin: 0

      Deterministic, real-time C framework with lock-free queues,
      O(1) memory pools and run-to-completion semantics.

   .. grid-item-card:: Configurable Footprint
      :margin: 0

      Compile-time switches strip function pointers, debug
      infrastructure and counters for sub-1 KB RAM targets.


.. toctree::
   :maxdepth: 2
   :caption: Getting Started

   getting_started/installation
   getting_started/quickstart
   getting_started/first_module

.. toctree::
   :maxdepth: 2
   :caption: Architecture

   architecture/overview
   architecture/crf_runtime
   architecture/event_model
   architecture/memory_management

.. toctree::
   :maxdepth: 2
   :caption: C API Reference

   c_api/chsm
   c_api/cevent
   c_api/cqueue
   c_api/cpool
   c_api/crf
   c_api/chsm_cfg

.. toctree::
   :maxdepth: 2
   :caption: GUI & Code Generation

   gui/editor
   gui/code_generation
   gui/new_module_wizard
   gui/debug_channel

.. toctree::
   :maxdepth: 2
   :caption: Configuration

   configuration/build_system
   configuration/runtime_tuning
   configuration/project_settings

.. toctree::
   :maxdepth: 1
   :caption: Appendices

   appendices/faq
   appendices/changelog
   appendices/license
