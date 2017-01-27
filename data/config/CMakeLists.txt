#copy config files where necessary

set(CONFIG_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(CONFIG_FILES "")
list(APPEND CONFIG_FILES "map.cfg")
list(APPEND CONFIG_FILES "main_character_anim.cfg")
list(APPEND CONFIG_FILES "main_character_atlas.cfg")
list(APPEND CONFIG_FILES "main_character_shader.cfg")
list(APPEND CONFIG_FILES "terrain_atlas.cfg")
list(APPEND CONFIG_FILES "terrain_shader.cfg")

foreach(CONFIG_FILE ${CONFIG_FILES})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE}
  ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE} COPYONLY)
endforeach()
