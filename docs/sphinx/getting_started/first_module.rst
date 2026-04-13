.. _first_module:

=================================
Creating Your First Module
=================================

The **New Module Wizard** scaffolds a complete CHSM module with state machine
code, CMake build file, headers, documentation stub and unit tests.

Launch the Wizard
==================

1. In the editor sidebar click **New Module**.
2. A new browser tab opens with the module creation form.

Fill in the Form
=================

.. list-table::
   :widths: 25 75
   :header-rows: 1

   * - Field
     - Description
   * - **Module name**
     - Identifier used for file names and C symbols (e.g. ``lm73``).
   * - **Module version**
     - Semantic version string (e.g. ``0.1.0``).
   * - **Author Nick / Full Name**
     - Attribution for file headers.
   * - **Description**
     - Brief text inserted into generated file comments.
   * - **Device Address**
     - I²C/SPI slave address (hex string, e.g. ``0x48``).
   * - **Communication peripheral**
     - ``I2C``, ``SPI``, ``CAN`` or ``None``.  Controls which protocol
       library is linked and included.
   * - **Licence**
     - ``MIT``, ``BSD-3``, ``GPL-3.0``, ``Apache-2.0`` or ``MPL-2.0``.
   * - **Module location**
     - Output directory.  Use **Browse** to pick a folder.
   * - **Package Name**
     - CMake package namespace (default ``chsm``).
   * - **Linked Libs**
     - Additional CMake link targets (space-separated).

Click **Generate**.  On success the button turns green and the following
tree is created:

::

   <module_name>/
   ├── CMakeLists.txt
   ├── doc/
   │   └── <module_name>.html
   ├── inc/
   │   ├── <module_name>.h
   │   ├── <module_name>_functions.h
   │   └── <module_name>_regs.h
   ├── src/
   │   ├── <module_name>.c
   │   └── <module_name>_functions.c
   └── test/
       ├── CMakeLists.txt
       ├── tinc/
       └── tsrc/
           ├── main.c
           └── ut_<module_name>_test.c

Next Steps
===========

* Open ``inc/<module_name>.h`` in the CHSM editor to start drawing the
  state machine.
* Implement action functions in ``src/<module_name>_functions.c``.
* Add the module to your top-level ``CMakeLists.txt`` with
  ``add_subdirectory(<module_name>)``.
* Build and run unit tests:

  .. code-block:: bash

     cmake --build . -t <module_name>_test
     ctest -VV -R <module_name>_test
