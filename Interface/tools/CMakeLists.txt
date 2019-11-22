add_library(TOOLS_IFACE OBJECT
  tools.cpp
  ParallelTaskQueue.cpp
  SerialTaskQueue.cpp)

install(FILES
  tools.hpp
  ParallelTaskQueue.hpp
  SerialTaskQueue.hpp
  TaskQueue.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/interface/tools)

target_link_libraries(TOOLS_IFACE
  PUBLIC
  Boost
  TOOLS_IMPL)
