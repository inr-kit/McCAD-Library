add_subdirectory(common)
add_subdirectory(1D)
add_subdirectory(2D)
add_subdirectory(3D)

include_directories(
  "${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates/common"
  "${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates/1D"
  "${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates/2D"
  "${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates/3D")

add_library(Coordinates_IMPL OBJECT)

target_sources(Coordinates_IMPL PUBLIC
  $<TARGET_OBJECTS:CoordCommon_IMPL>
  $<TARGET_OBJECTS:Coord1D_IMPL>
  $<TARGET_OBJECTS:Coord2D_IMPL>
  $<TARGET_OBJECTS:Coord3D_IMPL>)
