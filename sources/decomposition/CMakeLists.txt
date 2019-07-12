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

add_library(DecomposeSolid decomposeSolid.cpp)
install (TARGETS DecomposeSolid DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(DecomposeSolid_impl decomposeSolid_impl.cpp)
install (TARGETS DecomposeSolid_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(Decomposition decomposition.cpp)
install (TARGETS Decomposition DESTINATION ${PROJECT_BINARY_DIR}/bin)
add_library(Decomposition_impl decomposition_impl.cpp)
install (TARGETS Decomposition_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)



