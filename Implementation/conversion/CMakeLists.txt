
add_library(CONVERSION_IMPL OBJECT
  voidGenerator.cpp
  BVHCreator.cpp
  conversion_impl.cpp)

install(FILES
  voidGenerator.hpp
  BVHCreator.hpp
  conversion_impl.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/conversion)

target_link_libraries(CONVERSION_IMPL
    OCC
    DECOMPOSITION_IMPL
    TOOLS_IFACE)
