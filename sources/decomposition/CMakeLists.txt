add_library(Edge edge.cpp)
install (TARGETS Edge DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(Edge_impl edge_impl.cpp)
install (TARGETS Edge_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(Meshtriangle meshtriangle.cpp)
install (TARGETS Meshtriangle DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(Meshtriangle_impl meshtriangle_impl.cpp)
install (TARGETS Meshtriangle_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(Surface surface.cpp)
install (TARGETS Surface DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(Surface_impl surface_impl.cpp)
install (TARGETS Surface_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(BoundSurface boundSurface.cpp)
install (TARGETS BoundSurface DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(BoundSurface_impl boundSurface_impl.cpp)
install (TARGETS BoundSurface_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(BoundSurfacePlane boundSurfacePlane.cpp)
install (TARGETS BoundSurfacePlane DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(BoundSurfacePlane_impl boundSurfacePlane_impl.cpp)
install (TARGETS BoundSurfacePlane_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(SplitSurfaces splitsurfaces.cpp)
install (TARGETS SplitSurfaces DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(SplitSurfaces_impl splitsurfaces_impl.cpp)
install (TARGETS SplitSurfaces_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(SplitSolid splitsolid.cpp)
install (TARGETS SplitSolid DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(SplitSolid_impl splitsolid_impl.cpp)
install (TARGETS SplitSolid_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(Solid solid.cpp)
install (TARGETS Solid DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(Solid_impl solid_impl.cpp)
install (TARGETS Solid_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(DecomposeSolid decomposeSolid.cpp)
install (TARGETS DecomposeSolid DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(DecomposeSolid_impl decomposeSolid_impl.cpp)
install (TARGETS DecomposeSolid_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(Decomposition decomposition.cpp)
install (TARGETS Decomposition DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(Decomposition_impl decomposition_impl.cpp)
install (TARGETS Decomposition_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)



