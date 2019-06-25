add_library(STEPReader stepreader.cpp)
install (TARGETS STEPReader DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES stepreader.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)

add_library(STEPReader_impl stepreader_impl.cpp)
install (TARGETS STEPReader_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES stepreader_impl.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
