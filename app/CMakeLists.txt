add_executable(test_cabbage test.cpp)
target_link_libraries(test_cabbage
  ${BGFX_LIBRARIES}
  ${BX_LIBRARIES}
  ${BGFX_COMMON_LIBRARIES}
  X11
  dl
  GL
  engine)
