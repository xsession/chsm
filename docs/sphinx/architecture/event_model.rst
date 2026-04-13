.. _event_model:

========================
Event Model
========================

Events are the sole communication mechanism between state machines and
the outside world.  Every event carries a signal identifier and
garbage-collection metadata.

Base Event Structure
=====================

.. code-block:: c

   typedef struct cevent_tst {
       signal_t    sig;       /* event type identifier */
       gc_info_tst gc_info;   /* pool id + reference count */
   } cevent_tst;

``signal_t`` defaults to ``uint16_t`` and can be redefined via the
``SIGNAL_T`` preprocessor macro before including ``cevent.h``.

Signal Classes
===============

Signals are grouped into *classes* of ``CRF_SIGNAL_CLASS_SIZE`` (100)
each.  Two classes are reserved:

.. list-table::
   :widths: 35 15 50
   :header-rows: 1

   * - Constant
     - Value
     - Purpose
   * - ``C_SIG_NONE``
     - 0
     - Null signal — no event.
   * - ``C_SIG_INIT``
     - 1
     - Internal initialisation signal.
   * - ``CRF_SIGNAL_CLASS_MOD_INTERNAL``
     - 1
     - Module-internal signal class.
   * - ``CRF_SIGNAL_CLASS_START``
     - 2
     - First application-defined class.

Use the ``SIGNAL_FROM_CLASS`` macro to compute the base signal number
for a given class:

.. code-block:: c

   #define MY_CLASS   3
   enum my_signals {
       SIG_BUTTON_PRESS = SIGNAL_FROM_CLASS(MY_CLASS),  /* 300 */
       SIG_BUTTON_RELEASE,                               /* 301 */
   };

Derived Events
===============

Application events extend ``cevent_tst`` by placing it as the first
member:

.. code-block:: c

   typedef struct {
       cevent_tst super;
       uint8_t    button_id;
       uint32_t   timestamp;
   } button_event_tst;

The framework allocates and casts through the ``CRF_NEW`` and
``CRF_SIG_VAR`` macros:

.. code-block:: c

   /* Allocate */
   #define SIG_BUTTON_PRESS_TYPE   button_event_tst
   button_event_tst *e = CRF_NEW(SIG_BUTTON_PRESS);
   e->button_id  = 3;
   e->timestamp  = HAL_GetTick();
   CRF_EMIT(e);

   /* Receive */
   CRF_SIG_VAR(SIG_BUTTON_PRESS, btn, e_pst);
   process(btn->button_id);

Garbage Collection Metadata
============================

.. code-block:: c

   typedef struct {
       uint16_t ref_cnt : 12;   /* max 4095 references */
       uint16_t pool_id :  4;   /* pool 0 = static event */
   } gc_info_tst;

* **ref_cnt** — Incremented by ``cevent_ref_cnt_inc`` when an event is
  posted to a queue, decremented by ``cevent_ref_cnt_dec`` after
  dispatch.  When it reaches zero the framework returns the event to
  its pool.
* **pool_id** — Identifies the ``cpool_tst`` from which the event was
  allocated.  A ``pool_id`` of ``CEVENT_INVALID_POOL`` (0) indicates
  a static / const event that must never be freed.

Reference Counting Flow
========================

.. mermaid::

   sequenceDiagram
     participant App as Application
     participant CRF as crf
     participant Pool as cpool

     App->>CRF: CRF_NEW(SIG_X) → ref_cnt=0
     CRF->>Pool: cpool_new()
     Pool-->>CRF: event*
     CRF-->>App: event*
     App->>CRF: CRF_POST(event, &q) → ref_cnt++
     Note over CRF: dispatch to handler
     CRF->>CRF: crf_gc(event) → ref_cnt--
     alt ref_cnt == 0
       CRF->>Pool: cpool_gc(event) → return to free list
     end

Static Events
==============

Events that live for the entire program lifetime (e.g. timer ticks or
constant commands) can be declared as ``const`` globals with
``pool_id = 0``:

.. code-block:: c

   static const cevent_tst tick_event = {
       .sig     = SIG_TICK,
       .gc_info = {.ref_cnt = 0, .pool_id = CEVENT_INVALID_POOL}
   };

Static events are never garbage-collected regardless of their reference
count.
