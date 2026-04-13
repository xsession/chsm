.. _config_build_system:

==============================
CMake Build System
==============================

The C language port uses CMake with a set of helper macros defined in
``languages/c/cmake_utils/cmake_utils.cmake``.

Project Layout
===============

.. code-block:: text

   languages/c/
   ├── CMakeLists.txt            ← top-level project
   ├── cmake_utils/
   │   └── cmake_utils.cmake     ← helper macros
   ├── c_rf/                     ← CRF runtime library
   ├── unity/                    ← Unity test framework
   ├── modules/
   │   ├── chsm/                 ← HSM core module
   │   ├── lm73/                 ← example: temperature sensor
   │   ├── spi_master/           ← example: SPI bus master
   │   └── ...
   └── scripts/

Key CMake Macros
=================

``add_module_lib``
-------------------

.. code-block:: cmake

   add_module_lib(
       NAME        lm73
       SOURCES     src/lm73.c src/lm73_functions.c
       INCLUDES    inc
       DEPENDS     chsm c_rf
   )

Creates a static library target with correct include paths and
link dependencies.

``add_module_test``
--------------------

.. code-block:: cmake

   add_module_test(
       NAME        test_lm73
       SOURCES     test/test_lm73.c
       DEPENDS     lm73 unity
   )

Creates a test executable linked to Unity and the module under test.

Build Commands
===============

.. code-block:: bash

   cd languages/c
   cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ctest --test-dir build --output-on-failure

Cross-Compilation
==================

Pass a toolchain file for embedded targets:

.. code-block:: bash

   cmake -B build -S . \
       -DCMAKE_TOOLCHAIN_FILE=cmake_utils/compilers/arm-none-eabi.cmake \
       -DCMAKE_BUILD_TYPE=MinSizeRel

The ``compilers/`` directory contains pre-made toolchain files for
common embedded compilers (ARM GCC, IAR, MSVC for simulation).

Adding a New Module
====================

1. Use the New Module Wizard (see :ref:`gui_new_module_wizard`), or
   manually create the directory under ``languages/c/modules/``.
2. Add the module to the top-level ``CMakeLists.txt``:

   .. code-block:: cmake

      add_subdirectory(modules/my_module)

3. Build and test.
