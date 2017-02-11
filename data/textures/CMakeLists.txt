#TODO make script to compile textures to .ktx!!
set(TEXTURE_FILES "")
list(APPEND TEXTURE_FILES "character.ktx")
list(APPEND TEXTURE_FILES "character2.ktx")
list(APPEND TEXTURE_FILES "running_atlas.ktx")
list(APPEND TEXTURE_FILES "running_atlas_normals.ktx")
list(APPEND TEXTURE_FILES "test.ktx")
list(APPEND TEXTURE_FILES "map_atlas_color.ktx")
list(APPEND TEXTURE_FILES "map_atlas_color_8x8.ktx")
list(APPEND TEXTURE_FILES "map_atlas_normal.ktx")
list(APPEND TEXTURE_FILES "top_normal.ktx")
list(APPEND TEXTURE_FILES "top_rgb.ktx")

foreach(TEXTURE_FILE ${TEXTURE_FILES})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_FILE}
  ${CMAKE_CURRENT_BINARY_DIR}/${TEXTURE_FILE} COPYONLY)
endforeach()
