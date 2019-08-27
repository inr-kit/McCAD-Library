add_library(INPUTDATA
  inputdata.cpp)

install(FILES
  inputdata.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)
