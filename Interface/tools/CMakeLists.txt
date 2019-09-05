add_library(TOOLS_IFACE OBJECT
  tools.cpp)

install(FILES
  tools.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/interface/tools)

target_link_libraries(TOOLS_IFACE
  TOOLS_IMPL)
