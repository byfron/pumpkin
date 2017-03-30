add_library(clip2tri SHARED clip2tri.cpp)
target_link_libraries(clip2tri clipper poly2tri ${M_LIBRARY})

add_library(clipper SHARED clipper.cpp)


set(poly2tri_srcs
  common/shapes.cc
  sweep/advancing_front.cc
  sweep/cdt.cc
  sweep/sweep.cc
  sweep/sweep_context.cc
  )

add_library(poly2tri SHARED ${poly2tri_srcs})
target_link_libraries(poly2tri ${M_LIBRARY})