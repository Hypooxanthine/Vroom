# ----- Module definition -----

project(Window)
add_module(Window)

# ----- Dependencies -----

module_link_library(
  Window
  PUBLIC
    glfw
    glm::glm
)
module_link_modules(Window Core Event)

# ----- Compile definitions -----

module_compile_definition(
  Window
  PRIVATE VRM_WINDOW_EXPORTS=1
)

# ----- Compile options -----
