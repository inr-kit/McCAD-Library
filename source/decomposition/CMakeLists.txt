add_library(Decomposition decomposition.cpp)
install (TARGETS Decomposition DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES decomposition.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)

add_library(Decomposition_impl decomposition_impl.cpp)
install (TARGETS Decomposition_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES decomposition_impl.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)

add_library(DecomposeSolid decomposeSolid.cpp)
install (TARGETS DecomposeSolid DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES decomposeSolid.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)

add_library(DecomposeSolid_impl decomposeSolid_impl.cpp)
install (TARGETS DecomposeSolid_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES decomposeSolid_impl.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
