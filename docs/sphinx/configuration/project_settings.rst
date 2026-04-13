.. _config_project_settings:

==============================
Project Settings
==============================

The CHSM editor stores per-project configuration in JSON files alongside
the state-machine model.

``settings.json``
==================

Located at ``gui/c_gen/templates/settings.json``, this file provides
defaults for code generation:

.. code-block:: json

   {
     "output_dir": "build/gen",
     "language": "c",
     "indent": 4,
     "line_ending": "lf",
     "header_guard_style": "pragma_once"
   }

.. list-table::
   :widths: 25 15 60
   :header-rows: 1

   * - Key
     - Type
     - Description
   * - ``output_dir``
     - string
     - Relative path for generated files.
   * - ``language``
     - string
     - Default target language (``c``, ``python``, ``js``, ``java``, ``vhdl``).
   * - ``indent``
     - int
     - Indentation width in spaces.
   * - ``line_ending``
     - string
     - ``lf`` or ``crlf``.
   * - ``header_guard_style``
     - string
     - ``pragma_once`` or ``ifndef``.

``model.json``
===============

The template model file (``gui/c_gen/templates/model.json``) defines
the default structure for new state-machine projects:

.. code-block:: json

   {
     "name": "",
     "states": [],
     "transitions": [],
     "settings": {}
   }

Per-Module Overrides
=====================

Each module directory may contain its own ``settings.json`` that
overrides the global defaults.  The code generator merges settings
with module-level values taking precedence.

Theme Configuration
====================

Visual themes are CSS files under ``gui/web/themes/``.  To create a
custom theme:

1. Copy ``default.css`` to ``my_theme.css`` in the same directory.
2. Edit CSS custom properties:

   .. code-block:: css

      :root {
        --bg-primary: #1a1a2e;
        --text-primary: #e0e0e0;
        --accent: #0f3460;
        --state-fill: #16213e;
        --state-stroke: #0f3460;
        --transition-color: #e94560;
      }

3. The theme appears automatically in the sidebar theme selector.
