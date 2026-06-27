# ----- Module definition -----

project(Physics)
add_module(Physics)

# ----- Dependencies -----

module_link_modules(Physics Core)
module_link_library(Physics
  PRIVATE
    Jolt::Jolt
    glm::glm
)

# ----- Compile definitions -----

module_compile_definition(
  Physics
  PRIVATE VRM_PHYSICS_EXPORTS=1
)

# ----- Compile options -----
