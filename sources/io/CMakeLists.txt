add_library(IO
  stepreader.cpp
  stepreader_impl.cpp
  stepwriter.cpp
  stepwriter_impl.cpp)

install (TARGETS IO DESTINATION ${PROJECT_BINARY_DIR}/bin)
