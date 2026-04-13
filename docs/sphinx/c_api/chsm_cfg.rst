.. _api_chsm_cfg:

=======================================
``chsm_cfg`` — Build Configuration
=======================================

.. contents:: On this page
   :local:
   :depth: 2

Header
======

.. code-block:: c

   #include <chsm_cfg.h>

Include this header **before** any other CRF header, or pass the macros
as compiler flags (``-DCHSM_CFG_LITE``).

Configuration Macros
=====================

``CHSM_CFG_LITE``
-------------------

Strip virtual-dispatch function pointers from ``cqueue_tst``,
``cpool_tst``, and ``crf_tst``.  All calls become direct function
invocations, enabling the compiler to inline them.

**Impact:**

* Removes 4 function pointers from ``cqueue_tst`` (−16 B per queue on 32-bit).
* Removes 2 function pointers from ``cpool_tst`` (−8 B per pool).
* Removes 5 function pointers from ``crf_tst`` (−20 B per framework).
* Enables inlining of hot-path queue and pool operations.

``CHSM_CFG_NO_FAULT_CNT``
---------------------------

Remove the ``fault_cnt`` field from ``cqueue_tst``.

**Impact:**

* Saves 4 bytes per queue instance (2 queues per HSM → −8 B / HSM).
* Queue overflow is still reported via the ``-1`` return code; the
  counter is simply unavailable for diagnostics.

``CHSM_CFG_NO_DEBUG``
-----------------------

Omit debug-log function pointer and the state-name buffer from
generated code.

**Impact:**

* Approximately −20 bytes per module (varies with generated code).
* The TCP debug channel in the GUI will not receive state-change
  notifications from modules compiled without debug.

RAM Savings Summary
====================

The following table shows approximate RAM savings on a 32-bit
Cortex-M target with one HSM, one pool, and one framework instance.

.. list-table::
   :widths: 35 20 20 25
   :header-rows: 1

   * - Configuration
     - Queue (×2)
     - Pool
     - Framework
   * - Default (all features)
     - 56 B
     - 20 B
     - 48 B
   * - ``CHSM_CFG_LITE``
     - 24 B
     - 10 B
     - 12 B
   * - ``+ NO_FAULT_CNT``
     - 16 B
     - 10 B
     - 12 B
   * - ``+ NO_DEBUG``
     - 16 B
     - 10 B
     - 12 B

**Total savings** (all three flags): **~86 bytes** per HSM system.

Recommended Profiles
=====================

.. list-table::
   :widths: 20 40 40
   :header-rows: 1

   * - Profile
     - Flags
     - Use Case
   * - Development
     - *(none)*
     - Full diagnostics, debug logging, fault counters.
   * - Release
     - ``CHSM_CFG_LITE``
     - Reduced RAM, keep overflow diagnostics.
   * - Minimal
     - ``LITE + NO_FAULT_CNT + NO_DEBUG``
     - Smallest footprint for constrained MCUs (e.g. Cortex-M0 with ≤8 KB RAM).

CMake Integration
==================

.. code-block:: cmake

   target_compile_definitions(my_target PRIVATE
       CHSM_CFG_LITE
       CHSM_CFG_NO_FAULT_CNT
       CHSM_CFG_NO_DEBUG
   )

Or define them in a shared ``chsm_cfg.h`` included by all modules.
