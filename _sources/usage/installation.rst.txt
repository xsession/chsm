
Installing for CHSM
====================

Of the GUI and code generation
-------------------------------

#. Chrome. Theoretically other browsers can be used, but only Chrome was tested.
#. Python 3.7+
#. You can install the packages like this:

.. code-block:: bash

   pip3 install eel docopt

Of the C unit tests
--------------------
The C code uses the Unity unit testing framework for running the tests. The
CMake script that builds the tests assumes that the Unity and the CHSM repos
are in the same directory.

Cloning:

.. code-block:: bash

   git clone https://github.com/ThrowTheSwitch/Unity.git
