SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(BGFX_LIBRARIES ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbgfxRelease.a)
#SET(BX_LIBRARIES ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbxRelease.a)
SET(BX_LIBRARIES ${ROOT_DIR}/bx/.build/linux64_gcc/bin/libbxRelease.a)
SET(BGFX_COMMON_LIBRARIES ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libexample-commonRelease.a)
SET(SHADER_COMPILER ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/shadercRelease)

include_directories(${PROTOBUF_GENERATED_HEADERS})

message(STATUS ${PROTOBUF_GENERATED_HEADERS})

add_library(pumpkin_engine)
target_link_libraries(pumpkin_engine
  X11
  dl
  GL
  graphics_engine)


add_subdirectory(src)
add_subdirectory(data)
