# ----- Module definition -----

project(RenderObjects)
add_module(RenderObjects)

# ----- Compile definitions -----

module_link_modules(RenderObjects Rasterizer)
module_compile_definition(
  RenderObjects
  PRIVATE VRM_RENDEROBJECTS_EXPORTS=1
)

# ----- Compile options -----
