add_library(InputData inputdata.cpp)
install (TARGETS InputData DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES inputdata.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)

add_library(InputData_impl inputdata_impl.cpp)
install (TARGETS InputData_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES inputdata_impl.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
