add_library(utils_VertexUtils
  VertexUtils.hpp
  VertexUtils.cpp)


target_link_libraries(utils_VertexUtils
  ${BGFX_COMMON_LIBRARIES})