add_library(DECOMPOSITION_IFACE
  edge.cpp
  meshtriangle.cpp
  surface.cpp
  boundSurface.cpp
  boundSurfacePlane.cpp
  splitsurfaces.cpp
  splitsolid.cpp
  solid.cpp
  decomposeSolid.cpp
  decomposition.cpp)

install(FILES
  edge.hpp
  meshtriangle.hpp
  surface.hpp
  boundSurface.hpp
  boundSurfacePlane.hpp
  splitsurfaces.hpp
  splitsolid.hpp
  solid.hpp
  decomposeSolid.hpp
  decomposition.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)
