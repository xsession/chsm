.. _gui_editor:

=========================
Visual State Editor
=========================

The CHSM GUI is a browser-based visual editor for hierarchical state
machines.  It runs locally via `Eel <https://github.com/python-eel/Eel>`_
(Python ↔ Chromium bridge).

Launching
=========

.. code-block:: bash

   cd gui
   python chsm_backend.py

The editor opens in a Chromium window at ``http://localhost:8080``.

Canvas
======

The central drawing area renders states as rounded rectangles and
transitions as directional arrows.  Operations:

.. list-table::
   :widths: 25 75
   :header-rows: 1

   * - Action
     - Description
   * - Click + Drag
     - Move a state.
   * - Double-click state
     - Edit state name and entry/exit actions.
   * - Right-click state
     - Context menu — add child state, delete.
   * - Click + Drag between states
     - Create a transition arrow.
   * - Double-click transition
     - Edit label (``signal [guard] / action``).

Transition Labels
==================

Labels follow UML syntax:

.. code-block:: text

   signal [guard] / action

* **signal** — Event name (e.g. ``SIG_BUTTON``).
* **guard** — Optional boolean expression in brackets.
* **action** — Optional semicolon-separated list of actions.

Examples::

   SIG_TIMEOUT / led_off();
   SIG_DATA [len > 0] / process(); ack();

Sidebar
========

* **File** — Open / Save ``.chsm`` JSON model files.
* **Generate** — Trigger code generation for selected languages.
* **Settings** — Theme selection, grid toggle, snap-to-grid.
* **Debug** — Connect to the TCP debug channel (see :ref:`debug_channel`).

Themes
======

Three built-in themes are available under ``web/themes/``:

* ``default.css`` — Light theme.
* ``solarized_light.css`` — Solarized Light.
* ``solarized_dark.css`` — Solarized Dark.

Select a theme from the sidebar **Settings** panel.

Model Format
============

The editor serialises state machines as JSON.  The top-level structure:

.. code-block:: text

   {
     "name": "my_hsm",
     "states": [ ... ],
     "transitions": [ ... ],
     "settings": { ... }
   }

Each **state** contains:

* ``name``, ``id``, ``parent_id``
* ``entry``, ``exit`` — Action function names.
* ``x``, ``y``, ``w``, ``h`` — Canvas geometry.

Each **transition** contains:

* ``source_id``, ``target_id``
* ``signal``, ``guard``, ``actions``
* Waypoint coordinates for routing.
