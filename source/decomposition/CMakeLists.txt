add_library(Decomposition decomposition.cpp)
install (TARGETS Decomposition DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES decomposition.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)

add_library(Decomposition_impl decomposition_impl.cpp)
install (TARGETS Decomposition_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES decomposition_impl.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
