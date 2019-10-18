include_directories(
  "${PROJECT_SOURCE_DIR}/Implementation/geometry/Coordinates"
  "${PROJECT_SOURCE_DIR}/Implementation/geometry/Coordinates/common"
  "${PROJECT_SOURCE_DIR}/Implementation/geometry/Coordinates/1D"
  "${PROJECT_SOURCE_DIR}/Implementation/geometry/Coordinates/2D"
  "${PROJECT_SOURCE_DIR}/Implementation/geometry/Coordinates/3D")

add_subdirectory(Coordinates)

add_library(GEOMETRY_IMPL OBJECT
  edge_impl.cpp
  meshtriangle_impl.cpp
  surface_impl.cpp
  boundSurface_impl.cpp
  boundSurfacePlane_impl.cpp
  solid_impl.cpp
  planarSolid_impl.cpp
  cylSolid_impl.cpp
  boundingBox_impl.cpp)

install(FILES
  edge_impl.hpp
  meshtriangle_impl.hpp
  surface_impl.hpp
  boundSurface_impl.hpp
  boundSurfacePlane_impl.hpp
  solid_impl.hpp
  planarSolid_impl.hpp
  cylSolid_impl.hpp
  boundingBox_impl.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/geometry)

target_link_libraries(GEOMETRY_IMPL PUBLIC
  OCC)
