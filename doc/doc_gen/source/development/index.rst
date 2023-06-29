============
Development
============


Hierarchical overview
---------------------------

.. uml::

   Alice -> Bob: Hi!
   Alice <- Bob: How are you?

.. code-block:: text

    myproject/
    ├── conf.py
    ├── plantuml.jar
    ├── index.rst
    ├── _build/
    ├── _static/
    ├── _templates/
    └── modules/
        ├── module1.rst
        └── module2.rst