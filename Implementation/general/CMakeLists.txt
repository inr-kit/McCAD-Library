add_library(INPUTDATA
  inputdata_impl.cpp)

install(FILES
  inputdata_impl.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(INPUTDATA
  INPUTDATA_IFACE
  OCC_TOOLKITS)
