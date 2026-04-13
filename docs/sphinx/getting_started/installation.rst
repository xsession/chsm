.. _installation:

============
Installation
============

Prerequisites
=============

.. list-table::
   :widths: 30 70
   :header-rows: 1

   * - Component
     - Requirement
   * - Python
     - 3.7 or later
   * - Browser
     - Chromium-based (Chrome, Edge) recommended; Firefox supported
   * - C Compiler
     - Any C11-compliant compiler (GCC, Clang, ARMCC, IAR)
   * - CMake
     - 3.19 or later (for building modules and tests)

Install the GUI and Code Generator
===================================

1. Clone the repository:

   .. code-block:: bash

      git clone https://github.com/xsession/chsm.git
      cd chsm

2. Install Python dependencies:

   .. code-block:: bash

      pip install eel docopt cookiecutter

3. Launch the editor:

   .. code-block:: bash

      python gui/chsm_backend.py

   The editor opens in your default browser at ``http://127.0.0.1:<port>/main.html``.

Install the C Unit Test Framework
==================================

The C tests use the `Unity <https://github.com/ThrowTheSwitch/Unity>`_ test
framework.  Clone it beside the ``chsm`` directory:

.. code-block:: bash

   git clone https://github.com/ThrowTheSwitch/Unity.git

Build and run all tests:

.. code-block:: bash

   cd chsm/languages/c
   mkdir build && cd build
   cmake ..
   cmake --build .
   ctest -VV

Optional: Documentation Dependencies
======================================

To build this documentation locally:

.. code-block:: bash

   pip install -r docs/requirements.txt
   cd docs/sphinx
   make html          # HTML output in _build/html/

For PDF output via Typst:

.. code-block:: bash

   # Install Typst: https://github.com/typst/typst/releases
   cd docs/typst
   typst compile main.typ chsm_reference.pdf
