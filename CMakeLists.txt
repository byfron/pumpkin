SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(BGFX_LIBRARIES
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbgfxRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbxRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbimg_encodeRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbimg_decodeRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbimgRelease.a)

#SET(BX_LIBRARIES ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbxRelease.a)
#SET(BX_LIBRARIES ${ROOT_DIR}/bx/.build/linux64_gcc/bin/libbxRelease.a)
SET(BGFX_COMMON_LIBRARIES ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libexample-commonRelease.a)
#SET(BGIMG_LIBRARIES
#  ${ROOT_DIR}/bimg/.build/linux64_gcc/bin/libbimg_encodeRelease.a
#  ${ROOT_DIR}/bimg/.build/linux64_gcc/bin/libbimg_decodeRelease.a
#  ${ROOT_DIR}/bimg/.build/linux64_gcc/bin/libbimgRelease.a)
SET(SHADER_COMPILER ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/shadercRelease)

include_directories(${PROTOBUF_GENERATED_HEADERS})

message(STATUS ${PROTOBUF_GENERATED_HEADERS})

add_library(pumpkin_engine)
target_link_libraries(pumpkin_engine
  X11
  dl
  GL
  ${BGFX_LIBRARIES}
  ${BGFX_COMMON_LIBRARIES}
  graphics_engine)


add_subdirectory(src)
add_subdirectory(data)
