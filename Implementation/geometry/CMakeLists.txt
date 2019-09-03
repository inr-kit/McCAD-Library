add_subdirectory(Coordinates)

add_library(GEOMETRY_IMPL OBJECT
  edge_impl.cpp
  meshtriangle_impl.cpp
  surface_impl.cpp
  boundSurface_impl.cpp
  boundSurfacePlane_impl.cpp
  solid_impl.cpp
  BboundingBox.cpp)

install(FILES
  edge_impl.hpp
  meshtriangle_impl.hpp
  surface_impl.hpp
  boundSurface_impl.hpp
  boundSurfacePlane_impl.hpp
  solid_impl.hpp
  BoundingBox.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(GEOMETRY_IMPL
  Coordinates
  OCC)
