add_library(DECOMPOSITION_IMPL OBJECT
  edge_impl.cpp
  meshtriangle_impl.cpp
  surface_impl.cpp
  boundSurface_impl.cpp
  boundSurfacePlane_impl.cpp
  splitsurfaces_impl.cpp
  splitsolid_impl.cpp
  solid_impl.cpp
  decomposeSolid_impl.cpp
  decomposition_impl.cpp)

install(FILES
  edge_impl.hpp
  meshtriangle_impl.hpp
  surface_impl.hpp
  boundSurface_impl.hpp
  boundSurfacePlane_impl.hpp
  splitsurfaces_impl.hpp
  splitsolid_impl.hpp
  solid_impl.hpp
  decomposeSolid_impl.hpp
  decomposition_impl.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(DECOMPOSITION_IMPL
  OCC_TOOLKITS)
