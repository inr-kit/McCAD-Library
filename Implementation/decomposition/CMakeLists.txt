add_library(DECOMPOSITION_IMPL OBJECT
  preprocessor.cpp
  generateFacesList.cpp
  faceCollision.cpp
  splitsurfaces_impl.cpp
  splitsolid_impl.cpp
  decomposeSolid_impl.cpp
  decomposition_impl.cpp
  SolidRebuilder.cpp
  SolidSplitter.cpp
  SolidRepairer.cpp
  preprocessor.cpp
  generateFacesList.cpp
  faceCollision.cpp)

install(FILES
  preprocessor.hpp
  constructSolidObj.hpp
  generateFacesList.hpp
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
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/decomposition)

target_link_libraries(DECOMPOSITION_IMPL
  OCC)
