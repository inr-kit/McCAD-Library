add_library(InputData inputdata.cpp)
install (TARGETS InputData DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES inputdata.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
