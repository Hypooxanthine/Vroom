# ----- Module definition -----

project(Renderer)
add_module(Renderer)

# ----- Dependencies -----

module_link_modules(Renderer Core Tools Rasterizer RenderObjects AssetManager)

# ----- Compile definitions -----

module_compile_definition(
  Renderer
  PRIVATE VRM_RENDERER_EXPORTS=1
)

# ----- Compile options -----
