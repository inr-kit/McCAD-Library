add_library(Coord2D_IMPL OBJECT
  Coord2D.cpp
  Coord2D_const.cpp
  Coord2D_hash.cpp
  Coord2D_util.cpp)

install(FILES
  Coord2D.fwd
  Coord2D.hpp
  Coord2D_const.hpp
  Coord2D_hash.hpp
  Coord2D_util.hpp
  Coord2D.ipp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/geometry/coordinates/2d)

target_link_libraries(Coord2D_IMPL
  CoordCommon_IMPL
  Coord1D_IMPL)
