add_library(IO_IFACE OBJECT
  stepreader.cpp
  stepwriter.cpp
  Inputconfig.cpp)

install(FILES
  stepreader.hpp
  stepwriter.hpp
  Inputconfig.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/interface/io)

target_link_libraries(IO_IFACE
  IO_IMPL)
