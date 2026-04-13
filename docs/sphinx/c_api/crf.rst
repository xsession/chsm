.. _api_crf:

=================================
``crf`` — Reactive Framework
=================================

.. contents:: On this page
   :local:
   :depth: 2

Header
======

.. code-block:: c

   #include <crf.h>

Overview
========

``crf_tst`` is the top-level framework object.  It owns the array of
state machines and memory pools, drives the event loop, and provides
the event allocation / garbage-collection API.

A single global instance ``crf`` is declared in ``crf.c``.

Types
=====

``crf_tst``
------------

.. code-block:: c

   typedef struct crf_tst {
   #ifndef CHSM_CFG_LITE
       void* (*new_event)(struct crf_tst *, uint32_t, signal_t);
       void  (*publish)(struct crf_tst *, const cevent_tst *);
       void  (*post)(struct crf_tst *, cevent_tst *, cqueue_tst *);
       bool  (*step)(struct crf_tst *);
       void  (*gc)(struct crf_tst *, const cevent_tst *);
   #endif
       chsm_tst  **chsm_ap;         /* NULL-terminated HSM array */
       cpool_tst  *pool_ast;        /* pool array */
       uint16_t    pool_cnt_u16;    /* number of pools */
   } crf_tst;

Constants
=========

.. list-table::
   :widths: 40 60
   :header-rows: 1

   * - Name
     - Description
   * - ``CRF_MAX_POOL_COUNT``
     - Maximum pools per instance (default 4).

Functions
=========

``crf_init``
-------------

.. code-block:: c

   bool crf_init(crf_tst   *self,
                  chsm_tst **chsm_ap,
                  cpool_tst *pool_ast,
                  uint16_t   pool_cnt);

Initialise the framework.

:param self: Framework instance.
:param chsm_ap: NULL-terminated array of state machine pointers.
:param pool_ast: Array of memory pools.
:param pool_cnt: Number of pools (≤ ``CRF_MAX_POOL_COUNT``).
:returns: ``false`` on parameter error.

``crf_new_event``
------------------

.. code-block:: c

   void* crf_new_event(crf_tst *self,
                        uint32_t size,
                        signal_t sig);

Allocate an event from the first pool whose block size ≥ ``size``.

:param size: Required event size in bytes.
:param sig: Signal to write into ``cevent_tst.sig``.
:returns: Pointer to the new event, or ``NULL`` if no pool can serve
          the request.

The ``gc_info`` is initialised: ``ref_cnt = 0``, ``pool_id`` set to the
matching pool index + 1.

``crf_publish``
----------------

.. code-block:: c

   void crf_publish(crf_tst         *self,
                     const cevent_tst *e);

Placeholder for publish/subscribe.  Posts the event to **every**
registered state machine's event queue while incrementing ``ref_cnt``
accordingly.

``crf_post``
-------------

.. code-block:: c

   void crf_post(crf_tst    *self,
                  cevent_tst  *e,
                  cqueue_tst  *q);

Post an event to a specific queue (increments ``ref_cnt``).

``crf_step``
-------------

.. code-block:: c

   bool crf_step(crf_tst *self);

Run one processing cycle:

1. Iterate over all state machines in ``chsm_ap``.
2. For each HSM, dequeue one event.
3. Dispatch the event (``chsm_dispatch``).
4. Garbage-collect the event (``crf_gc``).

:returns: ``true`` if at least one event was processed.

``crf_gc``
-----------

.. code-block:: c

   void crf_gc(crf_tst         *self,
                const cevent_tst *e);

Decrement the event's ``ref_cnt``.  If it reaches zero, return the
event to its pool via ``cpool_gc``.  Static events (``pool_id == 0``)
are never freed.

Convenience Macros
===================

These macros operate on the global ``crf`` instance.

.. list-table::
   :widths: 40 60
   :header-rows: 1

   * - Macro
     - Expansion
   * - ``CRF_NEW_EVENT(type, sig)``
     - ``crf_new_event(&crf, sizeof(type), sig)``
   * - ``CRF_NEW(SIGNAL)``
     - ``CRF_NEW_EVENT(TYPEOF(SIGNAL), SIGNAL)``
   * - ``CRF_POST(e, q)``
     - ``crf_post(&crf, e, q)``
   * - ``CRF_STEP()``
     - ``crf_step(&crf)``
   * - ``CRF_GC(e)``
     - ``crf_gc(&crf, e)``
   * - ``CRF_POST_TO_SELF(e)``
     - Post to own queue with priority (``put_left``).
   * - ``CRF_EMIT(e)``
     - Call the active HSM's ``send`` callback.
   * - ``CRF_SIG_VAR(SIG, var, e)``
     - Declare and cast: ``TYPEOF(SIG) *var = (TYPEOF(SIG) *)e``
   * - ``TYPEOF(SIGNAL)``
     - Expands to ``SIGNAL##_TYPE``.

``CRF_SEND_FUNC`` / ``CRF_SET_SEND_FUNC``
-------------------------------------------

.. code-block:: c

   #define CRF_SEND_FUNC(hsm)     ((hsm)->send)
   #define CRF_SET_SEND_FUNC(hsm, fn)  ((hsm)->send = (fn))

Access or assign the ``send`` callback of a state machine.  The
``send`` function is called by ``CRF_EMIT`` and is typically wired
to a communication peripheral driver.

Usage Pattern
==============

.. code-block:: c

   /* Allocate event */
   my_event_tst *e = CRF_NEW(SIG_MY_EVENT);
   e->payload = 42;

   /* Route A — post to a specific queue */
   CRF_POST(e, &target_hsm.base.event_q_st);

   /* Route B — emit via send callback */
   CRF_EMIT(e);

   /* Route C — publish to all HSMs */
   crf.publish(&crf, (const cevent_tst *)e);
