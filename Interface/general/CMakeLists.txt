add_library(INPUTDATA_IFACE
  inputdata.cpp)

install(FILES
  inputdata.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(INPUTDATA_IFACE
  INPUTDATA)
