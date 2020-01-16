add_library(GEOMETRY_IFACE OBJECT
  edge.cpp
  meshtriangle.cpp
  surface.cpp
  boundSurface.cpp
  boundSurfacePlane.cpp
  boundSurfaceCyl.cpp
  solid.cpp
  planarSolid.cpp
  cylSolid.cpp)

install(FILES
  edge.hpp
  meshtriangle.hpp
  surface.hpp
  boundSurface.hpp
  boundSurfacePlane.hpp
  boundSurfaceCyl.hpp
  solid.hpp
  planarSolid.hpp
  cylSolid.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/interface/geometry)

target_link_libraries(GEOMETRY_IFACE
  GEOMETRY_IMPL)
