.. sectnum::
   :start: 2

Development
+++++++++++++

Overview
=========

Hierarchical
-------------

.. drawio-image:: ../_static/crf_SwAD.drawio
   :format: svg
   :page-index: 0

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

Functional
-----------

GUI
~~~~~

.. uml::

   Alice -> Bob: Hi!
   Alice <- Bob: How are you?

.. include:: ./theming.rst

Statemachine
~~~~~~~~~~~~~

