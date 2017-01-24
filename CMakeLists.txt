SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(BGFX_LIBRARIES ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbgfxRelease.a)
SET(BX_LIBRARIES ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbxRelease.a)
SET(BGFX_COMMON_LIBRARIES ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libexample-commonRelease.a)
SET(SHADER_COMPILER ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/shadercRelease)

include_directories(${PROTOBUF_GENERATED_HEADERS})

message(STATUS ${PROTOBUF_GENERATED_HEADERS})

add_library(cabbage)
target_link_libraries(cabbage
  ${BGFX_LIBRARIES}
  ${BX_LIBRARIES}
  ${BGFX_COMMON_LIBRARIES}
  X11
  dl
  GL
  engine)


#add_subdirectory(examples)
add_subdirectory(shaders)
add_subdirectory(src)
