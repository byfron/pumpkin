#TODO make script to compile textures to .ktx!!
set(SCRIPT_FILES "")
list(APPEND SCRIPT_FILES "test.py")

foreach(SCRIPT_FILE ${SCRIPT_FILES})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_FILE}
  ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT_FILE} COPYONLY)
endforeach()
