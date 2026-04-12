# ----- Module definition -----

project(TemplateProject)
add_module(TemplateProject)

# ----- Compile definitions -----

module_compile_definition(
  TemplateProject
  PRIVATE VRM_TEMPLATEPROJECT_EXPORTS=1
)

# ----- Compile options -----
