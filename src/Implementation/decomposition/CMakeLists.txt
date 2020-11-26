add_library(DECOMPOSITION_IMPL OBJECT
  surfaceObjCerator.cpp
  surfacesMerger.cpp
  pointOnSurface.cpp
  triangleCollision.cpp
  edgeOnSurface.cpp
  faceCollision.cpp
  EdgesCombiner.cpp
  CommonEdgeFinder.cpp
  AssistSurfaceGenerator.cpp
  splitsurfaces_impl.cpp
  splitsolid_impl.cpp
  preprocessor.cpp
  torusConvertor.cpp
  decomposeSolid_impl.cpp
  decomposition_impl.cpp
  SolidRebuilder.cpp
  SolidSplitter.cpp
  SolidRepairer.cpp)

install(FILES
  surfaceObjCerator.hpp
  surfacesMerger.hpp
  generateFacesList.hpp
  pointOnSurface.hpp
  pointInSolid.hpp
  triangleCollision.hpp
  edgeOnSurface.hpp
  faceCollision.hpp
  EdgesCombiner.hpp
  CommonEdgeFinder.hpp
  AssistSurfaceGenerator.hpp
  splitsurfaces_impl.hpp
  splitsolid_impl.hpp
  solidObjCreator.hpp
  preprocessor.hpp
  torusConvertor.hpp
  decomposeSolid_impl.hpp
  decomposition_impl.hpp
  SolidRebuilder.hpp
  SolidSplitter.hpp
  SolidRepairer.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/decomposition)

target_link_libraries(DECOMPOSITION_IMPL
  OCC
  TOOLS_IFACE)
