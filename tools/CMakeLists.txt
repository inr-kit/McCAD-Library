add_library(Tools tools.cpp)
install (TARGETS Tools DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES tools.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)

add_library(Tools_impl tools_impl.cpp)
install (TARGETS Tools_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES tools_impl.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
