add_library(Decomposition decomposition.cpp)
install (TARGETS Decomposition DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES decomposition.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
