.. _crf_runtime:

===========================
CRF Runtime Library
===========================

The C Reactive Framework (CRF) provides a deterministic active-object
runtime for embedded targets.

Design Principles
==================

* **Run-to-completion (RTC):** Each event is fully processed before the
  next one is dequeued.  No preemption within a state machine step.
* **Static memory only:** All queues and pools are pre-allocated.
  No ``malloc`` at runtime.
* **ISR-safe event posting:** ``cqueue_put`` is wait-free (atomic
  compare-and-swap) so interrupts can inject events safely.
* **Pre-calculated transitions:** The code generator computes exit/entry
  paths at generation time; the runtime does not walk the hierarchy.

Execution Model
================

.. mermaid::

   sequenceDiagram
     participant ISR as Interrupt / Thread
     participant Q as Event Queue
     participant CRF as crf_step()
     participant HSM as State Handler

     ISR->>Q: cqueue_put(event)
     loop Main loop
       CRF->>Q: cqueue_get()
       Q-->>CRF: event pointer
       CRF->>HSM: state_handler(self, event)
       HSM-->>CRF: HANDLED / TRANSITION / IGNORED
       CRF->>CRF: crf_gc(event)
     end

The ``crf_step()`` function iterates over all registered HSMs, dequeues one
event per HSM and dispatches it to the current state handler.  After
dispatch, events with a reference count of zero are returned to their pool.

Struct Relationships
=====================

.. mermaid::

   classDiagram
     class crf_tst {
       +chsm_tst **chsm_ap
       +cpool_tst *pool_ast
       +uint16_t pool_cnt_u16
       +crf_step() bool
       +crf_new_event() void*
       +crf_gc() void
     }
     class chsm_tst {
       +cqueue_tst event_q_st
       +cqueue_tst defer_q_st
       +state_handler_pft
       +send()
       +next : chsm_tst*
     }
     class cqueue_tst {
       +cevent_tst **events
       +uint16_t max
       +atomic head, tail
       +put() / get()
     }
     class cpool_tst {
       +uint8_t *pool
       +uint16_t esize, ecnt
       +atomic head
       +new() / gc()
     }
     class cevent_tst {
       +signal_t sig
       +gc_info_tst gc_info
     }
     crf_tst --> chsm_tst : manages
     crf_tst --> cpool_tst : owns
     chsm_tst --> cqueue_tst : embeds 2×
     cpool_tst --> cevent_tst : allocates
     cqueue_tst --> cevent_tst : stores pointers

Thread Safety
==============

.. list-table::
   :widths: 30 20 50
   :header-rows: 1

   * - Function
     - Safety
     - Notes
   * - ``cqueue_put``
     - ISR-safe
     - Atomic CAS on head; multiple producers allowed.
   * - ``cqueue_get``
     - Main-thread only
     - Single consumer assumed.
   * - ``cpool_new``
     - ISR-safe
     - CAS-based free-list pop.
   * - ``cpool_gc``
     - ISR-safe
     - CAS-based free-list push.
   * - ``crf_step``
     - Main-thread only
     - Iterates HSMs sequentially.
