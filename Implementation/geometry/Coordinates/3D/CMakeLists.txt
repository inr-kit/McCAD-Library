add_library(Coord3D_IMPL OBJECT
  Coord3D.cpp
  Coord3D_const.cpp
  Coord3D_hash.cpp
  Coord3D_util.cpp)

install(FILES
  Coord3D.fwd
  Coord3D.hpp
  Coord3D_const.hpp
  Coord3D_hash.hpp
  Coord3D_util.hpp
  Coord3D.ipp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/coordinates/3d)

target_link_libraries(Coord3D_IMPL
  CoordCommon_IMPL
  Coord1D_IMPL)
