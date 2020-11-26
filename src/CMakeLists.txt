# General library
include_directories(
  "${PROJECT_SOURCE_DIR}/src/Interface/general"
  "${PROJECT_SOURCE_DIR}/src/Implementation/general")
add_subdirectory (Implementation/general)
add_subdirectory (Interface/general)
add_library(INPUTDATA INTERFACE)
target_sources(INPUTDATA INTERFACE
  $<TARGET_OBJECTS:INPUTDATA_IFACE>
  $<TARGET_OBJECTS:INPUTDATA_IMPL>)

# IO library
include_directories(
  "${PROJECT_SOURCE_DIR}/src/Interface/io"
  "${PROJECT_SOURCE_DIR}/src/Implementation/io")
add_subdirectory (Implementation/io)
add_subdirectory (Interface/io)
add_library(IO INTERFACE)
target_sources(IO INTERFACE
  $<TARGET_OBJECTS:IO_IFACE>
  $<TARGET_OBJECTS:IO_IMPL>)

# Tools library
include_directories(
  "${PROJECT_SOURCE_DIR}/src/Interface/tools"
  "${PROJECT_SOURCE_DIR}/src/Implementation/tools")
add_subdirectory (Implementation/tools)
add_subdirectory (Interface/tools)
add_library(TOOLS INTERFACE)
target_sources(TOOLS INTERFACE
  $<TARGET_OBJECTS:TOOLS_IFACE>
  $<TARGET_OBJECTS:TOOLS_IMPL>)

# Geometry library
include_directories(
  "${PROJECT_SOURCE_DIR}/src/Interface/geometry"
  "${PROJECT_SOURCE_DIR}/src/Implementation/geometry")
add_subdirectory (Implementation/geometry)
add_subdirectory (Interface/geometry)
add_library(GEOMETRY INTERFACE)
target_sources(GEOMETRY INTERFACE
  $<TARGET_OBJECTS:GEOMETRY_IFACE>
  $<TARGET_OBJECTS:GEOMETRY_IMPL>)
target_link_libraries(GEOMETRY INTERFACE Coordinates_IMPL TOOLS)

# Decomposition library
include_directories(
  "${PROJECT_SOURCE_DIR}/src/Interface/decomposition"
  "${PROJECT_SOURCE_DIR}/src/Implementation/decomposition")
add_subdirectory (Implementation/decomposition)
add_subdirectory (Interface/decomposition)
add_library(DECOMPOSITION INTERFACE)
target_sources(DECOMPOSITION INTERFACE
  $<TARGET_OBJECTS:DECOMPOSITION_IFACE>
  $<TARGET_OBJECTS:DECOMPOSITION_IMPL>)
target_link_libraries(DECOMPOSITION INTERFACE GEOMETRY)

# Conversion library
include_directories(
  "${PROJECT_SOURCE_DIR}/src/Interface/conversion"
  "${PROJECT_SOURCE_DIR}/src/Implementation/conversion")
add_subdirectory (Implementation/conversion)
add_subdirectory (Interface/conversion)
add_library(CONVERSION INTERFACE)
target_sources(CONVERSION INTERFACE
  $<TARGET_OBJECTS:CONVERSION_IFACE>
  $<TARGET_OBJECTS:CONVERSION_IMPL>)
#target_link_libraries(DECOMPOSITION INTERFACE MOAB)


