add_library(INPUTDATA
  inputdata_impl.cpp)

install(FILES
  inputdata_impl.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)
