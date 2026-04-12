# ----- Module definition -----

project(Rasterizer)
add_module(Rasterizer)

# ----- Dependencies -----

module_link_library(Rasterizer PUBLIC
  glm::glm
  libglew_shared
)
module_link_modules(Rasterizer Core Tools)

# ----- Compile definitions -----

module_compile_definition(
  Rasterizer
  PRIVATE VRM_RASTERIZER_EXPORTS=1
)

# ----- Compile options -----
