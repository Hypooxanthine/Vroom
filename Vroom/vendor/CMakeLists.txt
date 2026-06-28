project(vendor)

include(FetchContent)

# ----- STB_IMAGE -----

add_library(
  StbImage
  STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/src/stb_image_impl.cpp
)

target_include_directories(
  StbImage
  SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include
)

# ----- Nlohmann::json -----

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Treat json headers as system too (see vrm_promote_system_includes)
vrm_promote_system_includes(nlohmann_json::nlohmann_json)
