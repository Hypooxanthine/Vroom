function(module_add_public_headers_check MODULE_NAME)

  # If A depends on B, A accesses B::protected
  # But users only access A::public and B::public
  # If A includes a protected header of B, user
  # code won't compile. We need to check that privacy
  # is being respected.

  file(GLOB PUBLIC_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/public/${MODULE_NAME}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/public/${MODULE_NAME}/*.hpp"
  )

  if (PUBLIC_HEADERS)

    add_executable(${MODULE_NAME}_headers_check EXCLUDE_FROM_ALL)
    target_sources(${MODULE_NAME}_headers_check PRIVATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/dummyCppFile.cpp ${PUBLIC_HEADERS})
    target_link_libraries(${MODULE_NAME}_headers_check PRIVATE ${MODULE_NAME}::${MODULE_NAME})

    set_target_properties(${MODULE_NAME}_headers_check PROPERTIES
      EXCLUDE_FROM_ALL TRUE
      EXCLUDE_FROM_DEFAULT_BUILD TRUE
      FOLDER "_Checks"
    )
    
  endif()

endfunction()

function(module_set_sources MODULE_NAME SOURCES)

  set(PRIVATE_TARGET ${MODULE_NAME}_private)
  target_sources(${PRIVATE_TARGET} PRIVATE ${SOURCES})

endfunction()

function(module_include_dirs MODULE_NAME)

  set(PRIVATE_TARGET   ${MODULE_NAME}_private)
  set(PUBLIC_TARGET    ${MODULE_NAME}_public)
  set(PROTECTED_TARGET ${MODULE_NAME}_protected)

  set(options)
  set(oneValueArgs PUBLIC PROTECTED PRIVATE LOCAL)
  cmake_parse_arguments(MOD "${options}" "${oneValueArgs}" "" ${ARGN})

  # An include dir is used but not exposed by the private target.
  # Only protected and public interfaces forward the include dirs.

  if(MOD_PRIVATE)
    target_include_directories(${PRIVATE_TARGET} PRIVATE ${MOD_PRIVATE})
  endif()
  
  if(MOD_PUBLIC)
    target_include_directories(${PRIVATE_TARGET} PRIVATE   ${MOD_PUBLIC})
    target_include_directories(${PUBLIC_TARGET}  INTERFACE ${MOD_PUBLIC})
  endif()

  if(MOD_PROTECTED)
    target_include_directories(${PRIVATE_TARGET}   PRIVATE   ${MOD_PROTECTED})
    target_include_directories(${PROTECTED_TARGET} INTERFACE ${MOD_PROTECTED})
  endif()

endfunction()

function(add_module MODULE_NAME)

  # Requirements:
  # Module dir tree:
  # ModuleName/
  # - private/ModuleName/*.h,*.hpp
  # - protected/ModuleName/*.h,*.hpp
  # - private/ModuleName/*.h,*.hpp
  # - src/*.cpp
    
  # Naming
  set(PRIVATE_TARGET   ${MODULE_NAME}_private)
  set(PROTECTED_TARGET ${MODULE_NAME}_protected)
  set(PUBLIC_TARGET    ${MODULE_NAME}_public)

  # --- Libraries creation ---

  add_library(${PRIVATE_TARGET} SHARED)
  set_target_properties(${PRIVATE_TARGET} PROPERTIES
    OUTPUT_NAME_DEBUG "vrm${MODULE_NAME}"
    DEBUG_POSTFIX     "d"
    PROJECT_LABEL     "${MODULE_NAME}"
    FOLDER            "Vroom"
  )

  add_library(${PUBLIC_TARGET} INTERFACE)
  # Public target interfaces the private target, but nothing is forwarded
  # bc private target should not expose anything at all, but interfaces do
  target_link_libraries(${PUBLIC_TARGET} INTERFACE ${PRIVATE_TARGET})
  add_library(${MODULE_NAME}::${MODULE_NAME} ALIAS ${PUBLIC_TARGET})

  add_library(${PROTECTED_TARGET} INTERFACE)
  target_link_libraries(${PROTECTED_TARGET} INTERFACE ${PUBLIC_TARGET})
  add_library(${MODULE_NAME}::protected ALIAS ${PROTECTED_TARGET})

  # ----- Module files -----

  file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
  file(GLOB_RECURSE PRIVATE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/private/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/private/*.hpp"
  )
  file(GLOB_RECURSE PROTECTED_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/protected/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/protected/*.hpp"
  )
  file(GLOB_RECURSE PUBLIC_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/public/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/public/*.hpp"
  )

  target_sources(${PRIVATE_TARGET} PRIVATE
    ${SOURCES}
    ${PRIVATE_HEADERS}
    ${PROTECTED_HEADERS}
    ${PUBLIC_HEADERS}
  )

  if (CMAKE_GENERATOR MATCHES "Visual Studio")
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
      ${SOURCES}
      ${PRIVATE_HEADERS}
      ${PROTECTED_HEADERS}
      ${PUBLIC_HEADERS}
    )
  endif()

  module_include_dirs(
    ${MODULE_NAME}
    PUBLIC    "${CMAKE_CURRENT_SOURCE_DIR}/public"
    PROTECTED "${CMAKE_CURRENT_SOURCE_DIR}/protected"
    PRIVATE   "${CMAKE_CURRENT_SOURCE_DIR}/private"
  )

  # ----- Compile options -----

  # Everyone
  if (MSVC)
    target_compile_options(${PRIVATE_TARGET} PRIVATE /MP)
  endif()
  
  module_add_public_headers_check(${MODULE_NAME})

endfunction()

function(module_link_modules MODULE_NAME)

  set(PRIVATE_TARGET       ${MODULE_NAME}_private)
  set(PROTECTED_TARGET     ${MODULE_NAME}_protected)
  set(PUBLIC_TARGET        ${MODULE_NAME}_public)

  foreach(DEP_MODULE IN LISTS ARGN)
    set(DEP_PROTECTED_TARGET ${DEP_MODULE}_protected)
    set(DEP_PUBLIC_TARGET    ${DEP_MODULE}_public)

    # Make the protected dep module usable by the module
    # Private linking so it doesn't leak through the public interface.
    target_link_libraries(${PRIVATE_TARGET} PRIVATE ${DEP_PROTECTED_TARGET})
    # Forward through the protected and public interface
    target_link_libraries(${PROTECTED_TARGET} INTERFACE ${DEP_PROTECTED_TARGET})
    target_link_libraries(${PUBLIC_TARGET}    INTERFACE ${DEP_PUBLIC_TARGET})
  endforeach()

endfunction()

function(module_link_library MODULE_NAME PRIVACY)

  set(PRIVATE_TARGET   ${MODULE_NAME}_private)
  set(PROTECTED_TARGET ${MODULE_NAME}_protected)
  set(PUBLIC_TARGET    ${MODULE_NAME}_public)

  if(PRIVACY STREQUAL "PRIVATE")
    target_link_libraries(${PRIVATE_TARGET} PRIVATE ${ARGN})
  elseif(PRIVACY STREQUAL "PROTECTED")
    target_link_libraries(${PRIVATE_TARGET}   PRIVATE   ${ARGN})
    target_link_libraries(${PROTECTED_TARGET} INTERFACE ${ARGN})
  elseif(PRIVACY STREQUAL "PUBLIC")
    target_link_libraries(${PRIVATE_TARGET} PRIVATE ${ARGN})
    target_link_libraries(${PUBLIC_TARGET}  INTERFACE ${ARGN})
  else()
    message(FATAL_ERROR "module_link_library(${MODULE_NAME}): privacy must be PRIVATE, PROTECTED, or PUBLIC")
  endif()

endfunction()

function(module_compile_definition MODULE_NAME)

  set(PRIVATE_TARGET   ${MODULE_NAME}_private)
  set(PROTECTED_TARGET ${MODULE_NAME}_protected)
  set(PUBLIC_TARGET    ${MODULE_NAME}_public)

  set(options)
  set(oneValueArgs PUBLIC PROTECTED PRIVATE LOCAL)
  cmake_parse_arguments(MOD "${options}" "${oneValueArgs}" "" ${ARGN})
  
  if(MOD_PUBLIC)
    target_compile_definitions(${PRIVATE_TARGET} PRIVATE   ${MOD_PUBLIC})
    target_compile_definitions(${PUBLIC_TARGET}  INTERFACE ${MOD_PUBLIC})
  endif()

  if(MOD_PROTECTED)
    target_compile_definitions(${PRIVATE_TARGET}   PRIVATE   ${MOD_PROTECTED})
    target_compile_definitions(${PROTECTED_TARGET} INTERFACE ${MOD_PROTECTED})
  endif()

  if(MOD_PRIVATE)
    target_compile_definitions(${PRIVATE_TARGET} PRIVATE ${MOD_PRIVATE})
  endif()

endfunction()
