
add_library(CONVERSION_IFACE OBJECT
  conversion.cpp)

install(FILES
  conversion.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/interface/conversion)

target_link_libraries(CONVERSION_IFACE)
