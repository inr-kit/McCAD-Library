add_library(DECOMPOSITION_IMPL OBJECT
  preprocessor.cpp
  generateFacesList.cpp
  pointOnSurface.cpp
  triangleCollision.cpp
  edgeOnSurface.cpp
  faceCollision.cpp
  splitsurfaces_impl.cpp
  splitsolid_impl.cpp
  decomposeSolid_impl.cpp
  decomposition_impl.cpp
  SolidRebuilder.cpp
  SolidSplitter.cpp
  SolidRepairer.cpp)

install(FILES
  pointOnSurface.hpp
  triangleCollision.hpp
  edgeOnSurface.hpp
  faceCollision.hpp
  splitsurfaces_impl.hpp
  splitsolid_impl.hpp
  decomposeSolid_impl.hpp
  decomposition_impl.hpp
  SolidRebuilder.hpp
  SolidSplitter.hpp
  SolidRepairer.hpp
  preprocessor.hpp
  createSolidObj.hpp
  generateFacesList.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/decomposition)

target_link_libraries(DECOMPOSITION_IMPL
  OCC)
