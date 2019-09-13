add_library(INPUTDATA_IMPL OBJECT
  inputdata_impl.cpp)

install(FILES
  inputdata_impl.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(INPUTDATA_IMPL
  OCC)
