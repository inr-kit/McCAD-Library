add_library(Decomposition decomposition.cpp)
install (TARGETS Decomposition DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(Decomposition_impl decomposition_impl.cpp)
install (TARGETS Decomposition_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(DecomposeSolid decomposeSolid.cpp)
install (TARGETS DecomposeSolid DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(DecomposeSolid_impl decomposeSolid_impl.cpp)
install (TARGETS DecomposeSolid_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
