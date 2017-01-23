add_library(engine
  Engine.hpp
  Engine.cpp
  Camera.hpp
  Camera.cpp
  # World.hpp
  # World.cpp
  # TileMap.hpp
  # TileMap.cpp
  TileMapChunk.hpp
  TileMapChunk.cpp
  InputManager.hpp
  InputManager.cpp)

target_link_libraries(engine
  utils_VertexUtils
  entities
  ${BGFX_COMMON_LIBRARIES}
  )

add_dependencies(engine shaders)
