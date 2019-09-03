add_library(GEOMETRY_IFACE OBJECT
  edge.cpp
  meshtriangle.cpp
  surface.cpp
  boundSurface.cpp
  boundSurfacePlane.cpp
  solid.cpp)

install(FILES
  edge.hpp
  meshtriangle.hpp
  surface.hpp
  boundSurface.hpp
  boundSurfacePlane.hpp
  solid.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(GEOMETRY_IFACE
  GEOMETRY_IMPL)
