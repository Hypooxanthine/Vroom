# ----- Module definition -----

project(ScriptEngine)
add_module(ScriptEngine)

# ----- Compile definitions -----

module_link_modules(ScriptEngine Core Tools)
module_link_library(ScriptEngine
  PRIVATE
    EnTT::EnTT
)
module_compile_definition(
  ScriptEngine
  PRIVATE VRM_SCRIPTENGINE_EXPORTS=1
)

# ----- Compile options -----
