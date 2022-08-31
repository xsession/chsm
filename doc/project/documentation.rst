.. _code-documentation:

Code Documentation
###################

API Documentation
******************

Well documented APIs enhance the experience for developers and are an essential
requirement for defining an API's success. Doxygen is a general purpose
documentation tool that the chsm project uses for documenting APIs. It
generates either an on-line documentation browser (in HTML) and/or provides
input for other tools that is used to generate a reference manual from
documented source files. In particular, doxygen's XML output is used as an input
when producing the CHSM project's online documentation.

Reference to Requirements
**************************

APIs for the most part document the implementation of requirements or advertised
features and can be traced back to features. We use the API documentation as the
main interface to trace implementation back to documented features. This is done
using custom _doxygen_ tags that reference requirements maintained somewhere
else in a requirement catalogue.

Test Documentation
*******************

To help understand what each test does and which functionality it tests we also
document all test code using the same tools and in the same context and generate
documentation for all unit and integration tests maintained in the same
environment. Tests are documented using references to the APIs or functionality
they validate by creating a link back to the APIs and by adding a reference to
the original requirements.


Documentation Guidelines
*************************

Test Code
=========
