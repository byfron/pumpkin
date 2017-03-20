#TODO make script to compile textures to .ktx!!
set(SCENE_FILES "")
list(APPEND SCENE_FILES "test2.fbx")

foreach(SCENE_FILE ${SCENE_FILES})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${SCENE_FILE}
  ${CMAKE_CURRENT_BINARY_DIR}/${SCENE_FILE} COPYONLY)
endforeach()