# ----- Module definition -----

project(Core)
add_module(Core)

# ----- Dependencies -----

module_link_library(Core PRIVATE 
  libglew_shared
  glfw
  glm::glm
)
module_link_library(Core PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# ----- Compile definitions -----

module_compile_definition(
  Core
  PRIVATE VRM_CORE_EXPORTS=1
)

# Everyone
module_compile_definition(Core PUBLIC GLM_ENABLE_EXPERIMENTAL=1)

# ----- Compile options -----
