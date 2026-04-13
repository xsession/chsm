.. _api_cpool:

==============================
``cpool`` — Memory Pool
==============================

.. contents:: On this page
   :local:
   :depth: 2

Header
======

.. code-block:: c

   #include <cpool.h>

Overview
========

``cpool_tst`` is a lock-free, fixed-size block allocator.  It manages a
contiguous byte buffer partitioned into equal-sized slots.  Free slots
are chained via an intrusive singly-linked list; allocation and
deallocation use atomic compare-and-swap.

Types
=====

``cpool_tst``
--------------

.. code-block:: c

   typedef struct cpool_tst {
   #ifndef CHSM_CFG_LITE
       void* (*new)(struct cpool_tst *);
       bool  (*gc)(struct cpool_tst *, const cevent_tst *);
   #endif
       uint8_t         *pool;
       uint16_t         esize;     /* bytes per block */
       uint16_t         ecnt;      /* total block count */
       atomic_uint16_t  head;      /* offset to first free block */
   } cpool_tst;

Constants
=========

.. list-table::
   :widths: 40 60
   :header-rows: 1

   * - Name
     - Description
   * - ``CPOOL_TERMINATOR`` (0xFFFF)
     - End-of-chain marker in the free list.

Functions
=========

``cpool_init``
---------------

.. code-block:: c

   void cpool_init(cpool_tst *self,
                    uint8_t   *buff,
                    uint16_t   event_size,
                    uint16_t   event_count);

Initialise the pool and thread the free list through ``buff``.

:param self: Pool instance.
:param buff: Pre-allocated buffer (``event_size × event_count`` bytes).
:param event_size: Size of each event block.
:param event_count: Number of blocks.

After initialisation the free list is:

.. code-block:: text

   head → [block 0] → [block 1] → … → [block N-1] → 0xFFFF

``cpool_new``
--------------

.. code-block:: c

   void* cpool_new(cpool_tst *self);

Allocate one block from the pool.

:returns: Pointer to the allocated block, or ``NULL`` if the pool is
          exhausted.

**Algorithm (CAS pop):**

1. ``old_head = atomic_load(self->head)``
2. If ``old_head == CPOOL_TERMINATOR`` → return ``NULL``
3. ``next = *(uint16_t *)(pool + old_head)``
4. ``CAS(&self->head, old_head, next)``
   — on failure, retry from step 1.

**Thread safety:** ISR-safe.

``cpool_gc``
-------------

.. code-block:: c

   bool cpool_gc(cpool_tst       *self,
                  const cevent_tst *e_pst);

Return an event block to the pool.

:param self: Pool instance.
:param e_pst: Event to free.
:returns: ``true`` if the event belonged to this pool and was freed,
          ``false`` otherwise.

**Algorithm (CAS push):**

1. Compute block offset from ``e_pst``.
2. ``old_head = atomic_load(self->head)``
3. Store ``old_head`` inside the freed block (next pointer).
4. ``CAS(&self->head, old_head, block_offset)``
   — on failure, retry from step 2.

**Thread safety:** ISR-safe.

Dispatch Macros
================

.. list-table::
   :widths: 30 35 35
   :header-rows: 1

   * - Macro
     - Normal mode
     - ``CHSM_CFG_LITE``
   * - ``CPOOL_NEW(p)``
     - ``(p)->new((p))``
     - ``cpool_new((p))``
   * - ``CPOOL_GC(p, e)``
     - ``(p)->gc((p), (e))``
     - ``cpool_gc((p), (e))``

Sizing Guidelines
==================

1. Count the peak number of events that can be in-flight simultaneously
   (queued + deferred + being dispatched).
2. Add a margin of 2–4 events for ISR bursts.
3. Choose ``event_size`` as the **largest** derived event struct that
   will be allocated from the pool.

.. code-block:: c

   /* Example: up to 12 simultaneous ADC events */
   #define ADC_POOL_SIZE   12
   static uint8_t adc_pool_buf[sizeof(adc_event_tst) * ADC_POOL_SIZE];
   static cpool_tst adc_pool;

   cpool_init(&adc_pool, adc_pool_buf,
              sizeof(adc_event_tst), ADC_POOL_SIZE);
