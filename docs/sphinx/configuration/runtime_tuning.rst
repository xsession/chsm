.. _config_runtime_tuning:

==============================
Runtime Tuning
==============================

CHSM provides three compile-time flags that trade features for smaller
RAM and flash footprint.  See :ref:`api_chsm_cfg` for the full API
reference.

Quick Reference
================

.. list-table::
   :widths: 30 15 55
   :header-rows: 1

   * - Flag
     - Default
     - Effect
   * - ``CHSM_CFG_LITE``
     - OFF
     - Replace function-pointer dispatch with direct calls.
   * - ``CHSM_CFG_NO_FAULT_CNT``
     - OFF
     - Remove queue overflow counter.
   * - ``CHSM_CFG_NO_DEBUG``
     - OFF
     - Strip debug instrumentation from generated code.

Choosing a Profile
===================

Development
-----------

Leave all flags **undefined**.  Full diagnostics are available:

* ``fault_cnt`` tracks queue overflows.
* Debug channel reports every state change.
* Function pointers allow mock injection in unit tests.

Release
--------

Define ``CHSM_CFG_LITE`` only:

.. code-block:: cmake

   target_compile_definitions(my_target PRIVATE CHSM_CFG_LITE)

* Queue / pool calls are inlined by the compiler.
* Fault counter remains for field diagnostics.
* Debug channel still operational.

Minimal (≤ 8 KB RAM MCUs)
---------------------------

Define all three flags:

.. code-block:: cmake

   target_compile_definitions(my_target PRIVATE
       CHSM_CFG_LITE
       CHSM_CFG_NO_FAULT_CNT
       CHSM_CFG_NO_DEBUG
   )

Approximate savings on Cortex-M0:

* **−32 B** per queue (×2 per HSM = −64 B).
* **−10 B** per pool.
* **−36 B** per framework instance.
* **−20 B** per module (debug buffers).

Queue Sizing
=============

Each HSM has two queues: **event** and **defer**.  Choose sizes as the
smallest power of 2 that satisfies the worst-case burst:

.. list-table::
   :widths: 30 20 50
   :header-rows: 1

   * - Queue
     - Typical size
     - Rationale
   * - Event queue
     - 8–16
     - Covers ISR burst between two ``crf_step`` calls.
   * - Defer queue
     - 4
     - Rarely more than 2–3 events deferred simultaneously.

Pool Sizing
============

1. Identify the largest derived event type.
2. Count peak in-flight events (queued + deferred + dispatching).
3. Add 2–4 blocks margin.

.. code-block:: c

   /* Worst case: 2 HSMs × 8 queue depth + 4 defer = 20 events */
   #define POOL_COUNT  24
   static uint8_t buf[sizeof(biggest_event_tst) * POOL_COUNT];
