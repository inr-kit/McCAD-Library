add_library(Universal universal.cpp)
install (TARGETS Universal DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES universal.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
