#TODO make script to compile textures to .ktx!!
set(TEXTURE_FILES "")
list(APPEND TEXTURE_FILES "character.ktx")
list(APPEND TEXTURE_FILES "test.ktx")

foreach(TEXTURE_FILE ${TEXTURE_FILES})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_FILE}
  ${CMAKE_CURRENT_BINARY_DIR}/${TEXTURE_FILE} COPYONLY)
endforeach()
