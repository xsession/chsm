.. _api_cevent:

=========================
``cevent`` — Event Base
=========================

.. contents:: On this page
   :local:
   :depth: 2

Header
======

.. code-block:: c

   #include <cevent.h>

Types
=====

``signal_t``
-------------

Event signal identifier.  Defaults to ``uint16_t``.

To override, define ``SIGNAL_T`` before including ``cevent.h``:

.. code-block:: c

   #define SIGNAL_T  uint8_t   /* save 1 byte per event */
   #include <cevent.h>

``gc_info_tst``
----------------

Garbage-collection metadata packed into 16 bits:

.. code-block:: c

   typedef struct {
       uint16_t ref_cnt : 12;   /* 0 – 4095 */
       uint16_t pool_id :  4;   /* 0 – 15   */
   } gc_info_tst;

``cevent_tst``
---------------

Base event structure — **all** events must start with this layout:

.. code-block:: c

   typedef struct cevent_tst {
       signal_t    sig;
       gc_info_tst gc_info;
   } cevent_tst;

Total size: 4 bytes (with default ``signal_t``).

Constants
=========

.. list-table::
   :widths: 40 15 45
   :header-rows: 1

   * - Name
     - Value
     - Description
   * - ``CEVENT_INVALID_POOL``
     - 0
     - Pool ID for static events (never freed).
   * - ``CRF_SIGNAL_CLASS_MOD_INTERNAL``
     - 1
     - Module-internal signal class.
   * - ``CRF_SIGNAL_CLASS_START``
     - 2
     - First application-defined signal class.
   * - ``CRF_SIGNAL_CLASS_SIZE``
     - 100
     - Signals per class.

Macros
======

``SIGNAL_FROM_CLASS``
----------------------

.. code-block:: c

   #define SIGNAL_FROM_CLASS(CLASS)  ((CLASS) * CRF_SIGNAL_CLASS_SIZE)

Compute the base signal value for a given class ID.

Functions
=========

``cevent_ref_cnt_inc``
-----------------------

.. code-block:: c

   static inline void
   cevent_ref_cnt_inc(const cevent_tst *e_pst);

Increment the event's reference count.  No-op if ``pool_id`` is
``CEVENT_INVALID_POOL``.

``cevent_ref_cnt_dec``
-----------------------

.. code-block:: c

   static inline void
   cevent_ref_cnt_dec(const cevent_tst *e_pst);

Decrement the event's reference count.  No-op if ``pool_id`` is
``CEVENT_INVALID_POOL``.

Deriving Custom Events
=======================

.. code-block:: c

   /* 1. Define the struct */
   typedef struct {
       cevent_tst super;     /* MUST be first member */
       int16_t    value;
       uint8_t    channel;
   } adc_event_tst;

   /* 2. Bind signal → type */
   #define SIG_ADC_READY_TYPE  adc_event_tst

   /* 3. Allocate */
   adc_event_tst *e = CRF_NEW(SIG_ADC_READY);
   e->value   = adc_read();
   e->channel = 4;
   CRF_EMIT(e);

   /* 4. Receive */
   CRF_SIG_VAR(SIG_ADC_READY, adc, e_pst);
   handle(adc->value, adc->channel);
