add_library(entities
  Animation2D.hpp
  Animation2D.cpp
  Entity2D.hpp
  Entity2D.cpp)

target_link_libraries(entities
  ${BGFX_COMMON_LIBRARIES})