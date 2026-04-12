project(vendor)

# ----- STB_IMAGE -----

add_library(
  StbImage
  STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/src/stb_image_impl.cpp
)

target_include_directories(
  StbImage
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include
)
