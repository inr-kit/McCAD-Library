add_library(CoordCommon_IMPL OBJECT
  coord_type_const.cpp
  coord_type_util.cpp)

install(FILES
  coord_type.hpp
  coord_type_const.hpp
  coord_type_util.hpp
  Dimension.hpp
  common.ipp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

target_link_libraries(CoordCommon_IMPL
  FloatingPoint)
