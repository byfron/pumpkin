add_library(graphics_engine
  ResourceManager.hpp
  ResourceManager.cpp
  GraphicsEngine.hpp
  GraphicsEngine.cpp
  Animation.hpp
  Animation.cpp
  GraphicsObject.hpp
  GraphicsObject.cpp
  ParticleObject.hpp
  ParticleObject.cpp
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
  Stencil.cpp
  Stencil.hpp)

target_link_libraries(graphics_engine
  utils_meshUtils
  utils_inputManager
  utils_camera
  utils_tilemapUtils
  fbxsdk
  proto
  clip2tri
  ${BGFX_COMMON_LIBRARIES}
  ${BX_LIBRARIES}
  ${BGFX_LIBRARIES}
)

add_dependencies(graphics_engine shaders)
