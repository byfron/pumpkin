add_library(graphics_engine
  ResourceManager.hpp
  ResourceManager.cpp
  GraphicsEngine.hpp
  GraphicsEngine.cpp
  Animation.hpp
  Animation.cpp
  GraphicsObject.hpp
  GraphicsObject.cpp
  Shader.hpp
  Shader.cpp
  TextureAtlas.hpp
  TextureAtlas.cpp
  TileMap.hpp
  TileMap.cpp
  TileMapLayer.hpp
  TileMapLayer.cpp
  )

target_link_libraries(graphics_engine
  utils_meshUtils
  utils_camera
  utils_inputManager
  ${BGFX_COMMON_LIBRARIES}
)

add_dependencies(graphics_engine shaders)
