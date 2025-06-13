cmake_minimum_required (VERSION 3.31)

# ----- Project definition -----

project(TestProject)

# ----- Subdirectories -----

add_subdirectory("Scripts")

# ----- Targets -----

add_custom_target(TestProject)

# add_custom_target(CopyEditor ALL
#   COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:VroomEditor> ${CMAKE_CURRENT_BINARY_DIR}/Output
#   COMMENT "Copying resource files to output directory"
# )


# ----- Deps -----

add_dependencies(TestProject ScriptsLibrary)

