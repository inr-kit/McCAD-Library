add_library(INPUTDATA
  inputdata.cpp
  inputdata_impl.cpp)

install (TARGETS INPUTDATA DESTINATION ${PROJECT_BINARY_DIR}/bin)
