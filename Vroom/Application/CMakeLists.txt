# ----- Module definition -----

project(Application)
add_module(Application)

# ----- Dependencies -----

module_link_modules(Application Core Scene Event Window)

# ----- Compile definitions -----

module_compile_definition(
  Application
  PRIVATE VRM_APPLICATION_EXPORTS=1
)

# ----- Compile options -----
