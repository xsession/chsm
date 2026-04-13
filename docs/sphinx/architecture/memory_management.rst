.. _memory_management:

============================
Memory Management
============================

CHSM targets bare-metal and RTOS environments where dynamic allocation
is forbidden.  All memory is statically pre-allocated; the runtime never
calls ``malloc``.

Pool Allocator — ``cpool``
===========================

Each ``cpool_tst`` manages a contiguous buffer partitioned into
equal-sized blocks.  Allocation and deallocation are lock-free (CAS
based).

Initialisation
--------------

.. code-block:: c

   #define EVT_SIZE   sizeof(button_event_tst)
   #define EVT_COUNT  8

   static uint8_t pool_buf[EVT_SIZE * EVT_COUNT];
   static cpool_tst pool;

   cpool_init(&pool, pool_buf, EVT_SIZE, EVT_COUNT);

``cpool_init`` threads a free-list through the buffer: each block stores
the byte offset to the next free block.  The terminator value is
``0xFFFF``.

Allocation — CAS Pop
---------------------

.. code-block:: text

   head ──► [block 0] ──► [block 1] ──► ... ──► 0xFFFF

   1. Read head (atomically).
   2. Compute next = *(uint16_t *)block[head].
   3. CAS(head, head, next).
      • Success: return block pointer.
      • Failure: retry from step 1.

Because the CAS loop is bounded to the pool size, ``cpool_new`` is
wait-free in practice.

Deallocation — CAS Push
-------------------------

.. code-block:: text

   1. Read head (atomically).
   2. Store head offset inside the freed block.
   3. CAS(head, old_head, freed_block_offset).
      • Success: done.
      • Failure: retry from step 1.

``cpool_gc`` additionally validates the pool by checking the event's
``pool_id`` against the pool array index.

Queue — ``cqueue``
====================

``cqueue_tst`` is a circular FIFO of ``cevent_tst`` pointers.  The
capacity **must** be a power of two for efficient masking.

Operations
-----------

.. list-table::
   :widths: 25 15 60
   :header-rows: 1

   * - Function
     - Producer
     - Behaviour
   * - ``cqueue_put``
     - ISR / Thread
     - Append to right (tail).  Atomic increment of ``head``.
   * - ``cqueue_put_left``
     - ISR / Thread
     - Prepend to left — used for priority / recalled events.
   * - ``cqueue_get``
     - Main only
     - Pop from left (tail side).  Single-consumer.
   * - ``cqueue_get_right``
     - Main only
     - Pop from right — used by ``chsm_recall``.

Overflow Handling
------------------

When the queue is full, ``cqueue_put`` returns ``-1`` and increments
``fault_cnt`` (unless disabled by ``CHSM_CFG_NO_FAULT_CNT``).
The event is **not** silently dropped — the caller can inspect the
return value and take corrective action.

Zero-Malloc Integration
=========================

A typical system declares all pools and queues statically:

.. code-block:: c

   /* Event buffers */
   static const cevent_tst *q_buf[16];
   static const cevent_tst *d_buf[4];

   /* Pool buffer */
   static uint8_t pool_buf[sizeof(my_event_tst) * 8];

   /* Instances */
   static cpool_tst  pools[1];
   static chsm_tst  *machines[] = {(chsm_tst *)&my_hsm, NULL};
   static crf_tst     crf;

   void init(void) {
       cpool_init(&pools[0], pool_buf,
                  sizeof(my_event_tst), 8);
       chsm_ctor((chsm_tst *)&my_hsm,
                  my_init_state, q_buf, 16, 4);
       crf_init(&crf, machines, pools, 1);
       chsm_init((chsm_tst *)&my_hsm);
   }

   void main_loop(void) {
       while (1) {
           crf_step(&crf);
           __WFI();           /* sleep until next interrupt */
       }
   }

RAM Budget
===========

.. list-table::
   :widths: 35 25 40
   :header-rows: 1

   * - Component
     - Size (bytes)
     - Notes
   * - ``cevent_tst``
     - 4
     - 2 (signal) + 2 (gc_info)
   * - ``cqueue_tst``
     - 28 / 16
     - 28 normal, 16 with ``CHSM_CFG_LITE``
   * - ``cpool_tst``
     - 20 / 10
     - 20 normal, 10 with ``CHSM_CFG_LITE``
   * - ``crf_tst``
     - 48 / 12
     - 48 normal, 12 with ``CHSM_CFG_LITE``
   * - Queue buffer
     - ``N × sizeof(void*)``
     - Pointer array — 4 B each on Cortex-M
   * - Pool buffer
     - ``N × event_size``
     - Aligned to event struct size
