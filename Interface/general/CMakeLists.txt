add_library(INPUTDATA_IFACE OBJECT
  inputdata.cpp)

install(FILES
  inputdata.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/interface/general)

target_link_libraries(INPUTDATA_IFACE
  INPUTDATA_IMPL)
