add_library(IO
  stepreader.cpp
  stepwriter.cpp)

install(FILES
  stepreader.hpp
  stepwriter.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)
