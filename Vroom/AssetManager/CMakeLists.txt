# ----- Module definition -----

project(AssetManager)
add_module(AssetManager)

# ----- Compile definitions -----

module_link_modules(AssetManager Core RenderObjects ScriptEngine)
module_link_library(AssetManager
  PRIVATE
    assimp
)
module_link_library(AssetManager
  PROTECTED
    nlohmann_json::nlohmann_json
    StbImage
)
module_compile_definition(
  AssetManager
  PRIVATE VRM_ASSETMANAGER_EXPORTS=1
)

# ----- Compile options -----
