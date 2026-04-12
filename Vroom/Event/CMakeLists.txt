# ----- Module definition -----

project(Event)
add_module(Event)

# ----- Compile definitions -----

module_link_library(
  Event
  PRIVATE
    glfw
)

module_link_modules(Event Core Tools)

module_compile_definition(
  Event
  PRIVATE VRM_EVENT_EXPORTS=1
)

# ----- Compile options -----
