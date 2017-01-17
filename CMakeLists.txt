cmake_minimum_required(VERSION 2.6.2)

project(cabbage)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -pthread -march=native")

SET(ROOT_DIR ${PROJECT_SOURCE_DIR})
SET(BGFX_LIBRARIES ${PROJECT_SOURCE_DIR}/bgfx/.build/linux64_gcc/bin/libbgfxRelease.a)
SET(BX_LIBRARIES ${PROJECT_SOURCE_DIR}/bgfx/.build/linux64_gcc/bin/libbxRelease.a)
SET(BGFX_COMMON_LIBRARIES ${PROJECT_SOURCE_DIR}/bgfx/.build/linux64_gcc/bin/libexample-commonRelease.a)
SET(SHADER_COMPILER ${ROOT_DIR}/bgfx/tools/bin/linux/shaderc)

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
