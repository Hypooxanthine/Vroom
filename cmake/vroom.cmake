
function(add_copy_dir_target TARGET_NAME FOR_TARGET DEST_DIR SOURCE_DIR)
  
  add_custom_target(${TARGET_NAME}
    COMMAND ${CMAKE_COMMAND} -E make_directory
      # Create /Resources dir
      ${DEST_DIR}
    
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      # Copy Resources dirs
      ${SOURCE_DIR}
      ${DEST_DIR}
    COMMENT "Copying resources from ${SOURCE_DIR} to ${DEST_DIR}"
  )

  add_dependencies(${FOR_TARGET} ${TARGET_NAME})

endfunction()

function(add_resources_target TARGET_NAME SOURCE_DIR)

  add_copy_dir_target(
    ${TARGET_NAME}
    VroomEditor
    $<TARGET_FILE_DIR:VroomEditor>/Resources
    ${SOURCE_DIR}
  )

endfunction()

function(set_scripts_library SCRIPTS_TARGET_NAME)

  target_compile_definitions(VroomEditor
    PRIVATE VRM_RUNTIME_SCRIPTS_PATH="$<TARGET_FILE:${SCRIPTS_TARGET_NAME}>"
  )
  
  add_dependencies(VroomEditor ${SCRIPTS_TARGET_NAME})
  
endfunction()
