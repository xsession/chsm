.. _gui_new_module_wizard:

===========================
New Module Wizard
===========================

The **New Module** wizard generates a complete, build-ready module
directory from a Cookiecutter template.

Opening the Wizard
===================

1. In the editor, click **File → New Module** (or press :kbd:`Ctrl+N`).
2. A dedicated browser page opens with the module creation form.

Form Fields
============

.. list-table::
   :widths: 25 15 60
   :header-rows: 1

   * - Field
     - Required
     - Description
   * - Module Name
     - Yes
     - Snake-case identifier (e.g. ``lm73``).
   * - Description
     - No
     - One-line summary.
   * - Author
     - No
     - Author name for file headers.
   * - Company
     - No
     - Company name for copyright headers.
   * - Licence
     - No
     - Licence identifier: ``MIT``, ``BSD``, ``Apache``, ``Proprietary``.
   * - Output Directory
     - Yes
     - Root path where the module folder is created.
   * - Comm Peripheral
     - Yes
     - ``i2c``, ``spi``, or ``none``.
   * - Unit Test
     - No
     - Generate Unity test scaffold (checkbox).
   * - Signal Class
     - No
     - Integer signal class ID for the module.
   * - Queue Size
     - No
     - Event queue depth (power of 2).
   * - Defer Queue Size
     - No
     - Defer queue depth (power of 2).

Communication Peripheral
=========================

When ``Comm Peripheral`` is set to ``i2c`` or ``spi``, the template
includes:

* A peripheral driver header include.
* Pre-defined I/O events (``SIG_TX_DONE``, ``SIG_RX_DONE``).
* A ``send`` callback stub wired to the peripheral HAL.

When set to ``none``, peripheral-specific code is omitted entirely.

Generated Directory Structure
==============================

.. code-block:: text

   <module_name>/
   ├── CMakeLists.txt
   ├── inc/
   │   └── <module_name>.h
   ├── src/
   │   └── <module_name>.c
   └── test/
       └── test_<module_name>.c     ← only if Unit Test checked

The generated ``CMakeLists.txt`` integrates with the project's
``add_module_lib`` / ``add_module_test`` CMake macros.

After Generation
=================

1. Copy or move the directory into ``languages/c/modules/``.
2. Open the ``.chsm`` model in the editor and draw states.
3. Generate code to populate the state-machine logic.
4. Build:

   .. code-block:: bash

      cd languages/c
      cmake -B build -S .
      cmake --build build
