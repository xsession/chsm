.. _api_chsm:

===========================
``chsm`` — State Machine
===========================

.. contents:: On this page
   :local:
   :depth: 2

Header
======

.. code-block:: c

   #include <chsm.h>

Types
=====

``chsm_tst``
-------------

Opaque state-machine instance.  Application state machines embed this
struct as their first member:

.. code-block:: c

   typedef struct my_hsm_tst {
       chsm_tst base;       /* must be first */
       uint32_t  counter;   /* application data */
   } my_hsm_tst;

``chsm_state_tpft``
---------------------

State handler function pointer:

.. code-block:: c

   typedef chsm_result_ten (*chsm_state_tpft)(
       chsm_tst         *self,
       const cevent_tst  *e_pst);

``chsm_user_func_tpft``
-------------------------

User action (entry / exit) function pointer:

.. code-block:: c

   typedef void (*chsm_user_func_tpft)(
       chsm_tst         *self,
       const cevent_tst  *e_pst);

``chsm_result_ten``
--------------------

Return value from state handlers:

.. list-table::
   :widths: 30 70
   :header-rows: 1

   * - Value
     - Meaning
   * - ``C_RES_HANDLED``
     - Event processed by this state.
   * - ``C_RES_TRANS``
     - Event triggers a state transition.
   * - ``C_RES_PARENT``
     - Delegate to parent state handler.
   * - ``C_RES_IGNORED``
     - Event intentionally ignored.
   * - ``C_RES_GUARDS``
     - Guard condition(s) evaluated — none true.

``chsm_call_ctx_st``
---------------------

Internal call context used during event dispatch.  Not normally accessed
by application code.

.. code-block:: c

   typedef struct {
       chsm_user_func_tpft exit_stack_apft[CHSM_MAX_NESTING_LEVEL];
       chsm_user_func_tpft *exit_ppft;
       chsm_state_tpft      start_pft;
   } chsm_call_ctx_st;

Constants
=========

.. list-table::
   :widths: 40 60
   :header-rows: 1

   * - Name
     - Description
   * - ``CHSM_MAX_NESTING_LEVEL``
     - Maximum hierarchy depth (default 8).
   * - ``C_SIG_NONE``
     - Null signal (0).
   * - ``C_SIG_INIT``
     - Initialisation signal (1).

Functions
=========

``chsm_ctor``
--------------

.. code-block:: c

   void chsm_ctor(chsm_tst       *self,
                   chsm_state_tpft init_state_pft,
                   const cevent_tst **events,
                   uint16_t        event_q_len,
                   uint16_t        defer_q_len);

Construct a state machine.

:param self: Pointer to the state-machine instance.
:param init_state_pft: Initial (top-level) state handler.
:param events: Pre-allocated pointer array shared by event & defer queues.
:param event_q_len: Size of the event queue (power of 2).
:param defer_q_len: Size of the defer queue (power of 2).

``chsm_init``
--------------

.. code-block:: c

   void chsm_init(chsm_tst *self);

Send the ``C_SIG_INIT`` event and execute the initial transition chain.
Call once after ``chsm_ctor`` and ``crf_init``.

``chsm_dispatch``
------------------

.. code-block:: c

   void chsm_dispatch(chsm_tst       *self,
                       const cevent_tst *e_pst);

Dispatch an event to the current state handler.  If the handler returns
``C_RES_PARENT``, the event is propagated up the hierarchy.

``chsm_defer``
---------------

.. code-block:: c

   void chsm_defer(chsm_tst       *self,
                    const cevent_tst *e_pst);

Move the current event to the defer queue.  The event's reference count
is incremented.

``chsm_recall``
----------------

.. code-block:: c

   void chsm_recall(chsm_tst       *self,
                     const cevent_tst *e_pst);

Move all events from the defer queue to the front of the main event
queue.

Inline Helpers
===============

``chsm_transition``
--------------------

.. code-block:: c

   static inline chsm_result_ten
   chsm_transition(chsm_tst *self, chsm_state_tpft target);

Record a transition target and return ``C_RES_TRANS``.  Used inside
state handlers:

.. code-block:: c

   return chsm_transition(self, my_target_state);

``chsm_handled``
-----------------

.. code-block:: c

   static inline chsm_result_ten chsm_handled(chsm_tst *self);

Return ``C_RES_HANDLED``.

``chsm_ignored``
-----------------

.. code-block:: c

   static inline chsm_result_ten chsm_ignored(chsm_tst *self);

Return ``C_RES_IGNORED``.
