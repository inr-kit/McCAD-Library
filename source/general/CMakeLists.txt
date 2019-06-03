add_library(General general.cpp)
install (TARGETS General DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES general.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
