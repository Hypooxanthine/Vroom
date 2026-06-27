# ----- Module definition -----

project(EditorModules)
add_module(EditorModules)

# ----- Dependencies -----

module_link_modules(EditorModules Core)
# module_link_library(EditorModules PUBLIC some_library)

# ----- Compile definitions -----

module_compile_definition(
  EditorModules
  PRIVATE VRM_EDITORMODULES_EXPORTS=1
)

# ----- Compile options -----
