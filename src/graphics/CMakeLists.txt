add_library(graphics_engine
  ResourceManager.hpp
  ResourceManager.cpp
  GraphicsEngine.hpp
  GraphicsEngine.cpp
  Animation.hpp
  Animation.cpp
  GraphicsObject.hpp
  GraphicsObject.cpp
  DebugGraphicsObject.hpp
  DebugGraphicsObject.cpp
  DebugManager.hpp
  DebugManager.cpp
  Shader.hpp
  Shader.cpp
  TextureAtlas.hpp
  TextureAtlas.cpp
  Scene.hpp
  Scene.cpp
        VisibilityLayer.cpp VisibilityLayer.hpp)

target_link_libraries(graphics_engine
  utils_meshUtils
  utils_inputManager
  utils_camera
  utils_tilemapUtils
  fbxsdk
  proto
  ${BGFX_COMMON_LIBRARIES}
  ${BX_LIBRARIES}
  ${BGFX_LIBRARIES}
)

add_dependencies(graphics_engine shaders)
