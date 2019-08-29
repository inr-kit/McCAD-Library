add_library(IO_IFACE
  stepreader.cpp
  stepwriter.cpp)

install(FILES
  stepreader.hpp
  stepwriter.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(IO_IFACE
  IO)
