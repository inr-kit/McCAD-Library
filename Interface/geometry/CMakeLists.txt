add_library(GEOMETRY_IFACE OBJECT
  edge.cpp
  meshtriangle.cpp
  surface.cpp
  boundSurface.cpp
  boundSurfacePlane.cpp
  boundSurfaceCyl.cpp
  boundSurfaceTor.cpp
  solid.cpp
  planarSolid.cpp
  cylSolid.cpp
  torSolid.cpp)

install(FILES
  edge.hpp
  meshtriangle.hpp
  surface.hpp
  boundSurface.hpp
  boundSurfacePlane.hpp
  boundSurfaceCyl.hpp
  boundSurfaceTor.hpp
  solid.hpp
  planarSolid.hpp
  cylSolid.hpp
  torSolid.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/interface/geometry)

target_link_libraries(GEOMETRY_IFACE
  GEOMETRY_IMPL)
