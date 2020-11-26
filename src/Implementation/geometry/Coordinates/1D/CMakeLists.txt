add_library(Coord1D_IMPL OBJECT
  Coord.cpp
  Coord_const.cpp
  Coord_hash.cpp
  Coord_util.cpp)

install(FILES
  Coord.fwd
  Coord.hpp
  Coord_const.hpp
  Coord_hash.hpp
  Coord_util.hpp
  Coord.ipp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/geometry/coordinates/1d)

target_link_libraries(Coord1D_IMPL
  CoordCommon_IMPL)
