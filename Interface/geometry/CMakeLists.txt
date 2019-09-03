add_library(DECOMPOSITION_IFACE OBJECT
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

target_link_libraries(DECOMPOSITION_IFACE
  DECOMPOSITION_IMPL)
