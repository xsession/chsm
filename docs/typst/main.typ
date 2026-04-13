// CHSM Reference Manual — Main Document
// Compile: typst compile main.typ

#import "template.typ": project

#show: project.with(
  title: "CHSM Reference Manual",
  subtitle: "C Hierarchical State Machine\nEditor & Runtime Framework",
  version: "1.0.0",
  author: "Janos Szeman",
)

#include "chapters/introduction.typ"
#include "chapters/architecture.typ"
#include "chapters/c_api.typ"
#include "chapters/code_generation.typ"
#include "chapters/configuration.typ"
