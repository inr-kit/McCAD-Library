add_library(TOOLS_IFACE
  tools.cpp)

install(FILES
  tools.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)
