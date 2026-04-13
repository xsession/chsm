.. _debug_channel:

==============================
Debug Channel
==============================

The debug channel provides real-time visibility into state-machine
execution via a TCP connection between the running firmware (simulated
or real) and the GUI editor.

Architecture
=============

.. mermaid::

   sequenceDiagram
     participant FW as Firmware / Sim
     participant TCP as TCP Socket
     participant BE as chsm_backend.py
     participant FE as Browser Debug Panel

     FW->>TCP: JSON message
     TCP->>BE: DebugServer receives
     BE->>FE: eel.debug_message(data)
     FE->>FE: Update panel

The ``DebugServer`` class in ``chsm_backend.py`` listens on a
configurable TCP port (default **9999**).  Each line received is a
JSON-encoded debug message.

Message Protocol
=================

All messages are single-line JSON objects terminated by ``\n``.

State Change
-------------

.. code-block:: json

   {
     "type": "state_change",
     "module": "lm73",
     "from": "idle",
     "to": "reading",
     "signal": "SIG_START_READ"
   }

Event Dispatch
---------------

.. code-block:: json

   {
     "type": "event",
     "module": "lm73",
     "signal": "SIG_RX_DONE",
     "pool_id": 1,
     "ref_cnt": 0
   }

Log
----

.. code-block:: json

   {
     "type": "log",
     "level": "info",
     "message": "Pool 1: 3/8 blocks used"
   }

GUI Debug Panel
================

The debug panel appears in the editor sidebar under the **Debug** tab.

Features:

* **Live state indicator** — Highlights the current state on the canvas.
* **Event log** — Scrollable list of dispatched events with timestamps.
* **Filter** — Filter by module name or signal.
* **Clear** — Reset the log.

Connecting a Target
====================

Embedded firmware sends debug messages over UART → TCP bridge (e.g. via
a host-side serial-to-socket script):

.. code-block:: bash

   # Example: pipe /dev/ttyUSB0 to TCP 9999
   socat TCP-LISTEN:9999,reuseaddr FILE:/dev/ttyUSB0,b115200

For desktop simulation, connect directly to ``localhost:9999``:

.. code-block:: python

   import socket, json

   sock = socket.socket()
   sock.connect(("localhost", 9999))
   msg = {"type": "state_change", "module": "lm73",
          "from": "idle", "to": "reading",
          "signal": "SIG_START_READ"}
   sock.sendall((json.dumps(msg) + "\n").encode())
   sock.close()

Disabling Debug in Firmware
=============================

Compile with ``CHSM_CFG_NO_DEBUG`` to strip all debug instrumentation
from the generated C code.  See :ref:`api_chsm_cfg`.
