add_library(DECOMPOSITION_IFACE OBJECT
  splitsurfaces.cpp
  splitsolid.cpp
  decomposeSolid.cpp
  decomposition.cpp)

install(FILES
  splitsurfaces.hpp
  splitsolid.hpp
  decomposeSolid.hpp
  decomposition.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/interface/decomposition)

target_link_libraries(DECOMPOSITION_IFACE
  DECOMPOSITION_IMPL)
