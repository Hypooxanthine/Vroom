# ----- Module definition -----

project(Tools)
add_module(Tools)

# ----- Compile definitions -----

module_link_modules(Tools Core)
module_compile_definition(
  Tools
  PRIVATE VRM_TOOLS_EXPORTS=1
)

# ----- Compile options -----
