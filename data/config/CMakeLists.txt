#copy config files where necessary

set(CONFIG_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(CONFIG_FILES "")
list(APPEND CONFIG_FILES "shaders.cfg")
list(APPEND CONFIG_FILES "scene.cfg")
list(APPEND CONFIG_FILES "textures.cfg")
list(APPEND CONFIG_FILES "map.cfg")
list(APPEND CONFIG_FILES "main_character_anim.cfg")
list(APPEND CONFIG_FILES "main_character_anim2.cfg")
list(APPEND CONFIG_FILES "main_character_anim3.cfg")
list(APPEND CONFIG_FILES "mask_data.txt")
list(APPEND CONFIG_FILES "map_data.txt")

foreach(CONFIG_FILE ${CONFIG_FILES})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE}
  ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE} COPYONLY)
endforeach()
