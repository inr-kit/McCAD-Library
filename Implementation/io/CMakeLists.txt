add_library(IO
  stepreader_impl.cpp
  stepwriter_impl.cpp)

install(FILES
  stepreader_impl.hpp
  stepwriter_impl.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(IO
  IO_IFACE
  OCC_TOOLKITS)
