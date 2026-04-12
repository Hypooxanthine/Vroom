# ----- Module definition -----

project(Scene)
add_module(Scene)

# ----- Compile definitions -----

module_link_modules(Scene Core Tools ScriptEngine AssetManager Renderer Event)
module_link_library(
  Scene
  PRIVATE
    glm::glm
)
module_link_library(
  Scene
  PUBLIC
    EnTT::EnTT
)
module_compile_definition(
  Scene
  PRIVATE VRM_SCENE_EXPORTS=1
)

# ----- Compile options -----
