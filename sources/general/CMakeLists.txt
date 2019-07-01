add_library(InputData inputdata.cpp)
install (TARGETS InputData DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(InputData_impl inputdata_impl.cpp)
install (TARGETS InputData_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
