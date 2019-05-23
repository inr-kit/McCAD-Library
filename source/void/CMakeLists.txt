add_library(Void void.cpp)
install (TARGETS Void DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES void.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
