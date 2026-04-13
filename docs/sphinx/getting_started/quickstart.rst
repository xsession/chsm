.. _quickstart:

===========
Quick Start
===========

This guide walks through creating a simple two-state machine and generating
C code from it.

Step 1 — Launch the Editor
===========================

.. code-block:: bash

   python gui/chsm_backend.py

A browser window opens with an empty canvas.

Step 2 — Open a Header File
=============================

Click **Open** in the sidebar and select an existing ``.h`` file that declares
your state machine's top-level handler, for example:

.. code-block:: c

   /* my_module.h */
   #ifndef MY_MODULE_H
   #define MY_MODULE_H

   #include "crf.h"

   typedef struct my_module_tst {
       chsm_tst super;
       /* module-specific fields */
   } my_module_tst;

   chsm_result_ten my_module_top(chsm_tst *self, const cevent_tst *e_pst);

   #endif

The editor parses the header and creates a canvas with the ``__top__`` state.

Step 3 — Draw States
=====================

* **Add a state:** Double-click on the canvas inside the ``__top__`` boundary.
* **Rename a state:** Select the state, type the new name in the *Label*
  field, click *Apply*.
* **Add an initial transition:** Double-click to create an *initial node*,
  then drag from it to the entry state.

Step 4 — Add Transitions
==========================

1. Hover the source state until a connector appears.
2. Drag from the connector to the target state.
3. Select the transition and set its label using the pattern:

   ::

      signal [guard()] / action()

   Examples: ``SIG_TICK / increment()``, ``SIG_DONE [is_ready()]``.

Step 5 — Generate Code
========================

Click **Code gen** in the sidebar.  Two files are produced:

* ``src/<module>.c`` — State handler functions with switch-case dispatch.
* ``inc/<module>_functions.h`` — Prototypes for user action / guard functions.

Step 6 — Integrate with CRF
=============================

.. code-block:: c

   #include "crf.h"
   #include "my_module.h"

   /* Event queue storage */
   static const cevent_tst *my_module_events[8];

   /* Instantiate the state machine */
   my_module_tst my_module_st;

   /* All HSMs in the application */
   chsm_tst *hsm_list[] = {(chsm_tst *)&my_module_st, NULL};

   /* Construct and initialise */
   chsm_ctor((chsm_tst *)&my_module_st, my_module_top,
             my_module_events, 8, 0);
   crf_init(&crf, hsm_list, NULL, 0);
   chsm_init((chsm_tst *)&my_module_st);

   /* Main loop */
   while (1) {
       crf_step(&crf);
   }
