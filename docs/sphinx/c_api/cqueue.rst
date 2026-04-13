.. _api_cqueue:

==============================
``cqueue`` — Event Queue
==============================

.. contents:: On this page
   :local:
   :depth: 2

Header
======

.. code-block:: c

   #include <cqueue.h>

Overview
========

``cqueue_tst`` is a circular FIFO of ``cevent_tst`` pointers.  It supports
multiple **interrupt-safe** producers (``put``, ``put_left``) and a
single main-thread consumer (``get``, ``get_right``).

The capacity **must** be a power of two; the implementation uses bitmask
wrapping instead of modulo.

Types
=====

``cqueue_tst``
---------------

.. code-block:: c

   typedef struct cqueue_tst {
   #ifndef CHSM_CFG_LITE
       int32_t           (*put)(struct cqueue_tst *, const cevent_tst *);
       int32_t           (*put_left)(struct cqueue_tst *, const cevent_tst *);
       const cevent_tst* (*get)(struct cqueue_tst *);
       const cevent_tst* (*get_right)(struct cqueue_tst *);
   #endif
       const cevent_tst **events;
       uint16_t           max;
       atomic_uint16_t    head;
       atomic_uint16_t    tail;
       uint16_t           mask;
   #ifndef CHSM_CFG_NO_FAULT_CNT
       uint32_t           fault_cnt;
   #endif
   } cqueue_tst;

Functions
=========

``cqueue_init``
----------------

.. code-block:: c

   int32_t cqueue_init(cqueue_tst      *self,
                        const cevent_tst **events,
                        uint16_t          max_event_count);

Initialise a queue.

:param self: Queue instance.
:param events: Pre-allocated pointer array of size ``max_event_count``.
:param max_event_count: Capacity (must be power of 2).
:returns: ``0`` on success.

``cqueue_put``
---------------

.. code-block:: c

   int32_t cqueue_put(cqueue_tst      *self,
                       const cevent_tst *e);

Enqueue an event at the **right** (tail) end.

:returns: ``0`` on success, ``-1`` if the queue is full.

**Thread safety:** ISR-safe (atomic increment of ``head``).

``cqueue_put_left``
--------------------

.. code-block:: c

   int32_t cqueue_put_left(cqueue_tst      *self,
                            const cevent_tst *e_pst);

Insert an event at the **left** (head) end — used for high-priority or
recalled events.

:returns: ``0`` on success, ``-1`` if the queue is full.

**Thread safety:** ISR-safe.

``cqueue_get``
---------------

.. code-block:: c

   const cevent_tst* cqueue_get(cqueue_tst *self);

Dequeue an event from the **left** (tail) end.

:returns: Event pointer, or ``NULL`` if empty.

**Thread safety:** Main-thread only (single consumer).

``cqueue_get_right``
---------------------

.. code-block:: c

   const cevent_tst* cqueue_get_right(cqueue_tst *self);

Dequeue an event from the **right** (head) end — used by
``chsm_recall`` to drain the defer queue.

:returns: Event pointer, or ``NULL`` if empty.

**Thread safety:** Main-thread only.

Dispatch Macros
================

When ``CHSM_CFG_LITE`` is defined, function-pointer dispatch is replaced
by direct calls:

.. list-table::
   :widths: 35 30 35
   :header-rows: 1

   * - Macro
     - Normal mode
     - ``CHSM_CFG_LITE``
   * - ``CQUEUE_PUT(q, e)``
     - ``(q)->put((q), (e))``
     - ``cqueue_put((q), (e))``
   * - ``CQUEUE_PUT_LEFT(q, e)``
     - ``(q)->put_left((q), (e))``
     - ``cqueue_put_left((q), (e))``
   * - ``CQUEUE_GET(q)``
     - ``(q)->get((q))``
     - ``cqueue_get((q))``
   * - ``CQUEUE_GET_RIGHT(q)``
     - ``(q)->get_right((q))``
     - ``cqueue_get_right((q))``

Always use the macros in application code to remain compatible with
both build modes.

Overflow Diagnostics
=====================

Unless ``CHSM_CFG_NO_FAULT_CNT`` is defined, every failed ``put`` /
``put_left`` increments ``fault_cnt``.  Monitor this counter during
development to size queues correctly:

.. code-block:: c

   if (my_hsm.base.event_q_st.fault_cnt > 0) {
       log_error("Queue overflow detected: %lu",
                 my_hsm.base.event_q_st.fault_cnt);
   }
