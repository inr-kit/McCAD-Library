cmake_minimum_required(VERSION 3.14.4 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(Use_Linux_OS "Linux OS is assumed by default. Change to OFF for Windows" ON)
option(MOAB_DIR "Path to MOAB directory")
message("--Use_Linux_OS: " ${Use_Linux_OS})

# Set project name
project(McCAD LANGUAGES CXX)

# Set version number
set (McCAD_VERSION_MAJOR 1)
set (McCAD_VERSION_MINOR 0)
set (McCAD_VERSION_RELEASE 0)

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/McCADConfig.h.in"
  "${PROJECT_BINARY_DIR}/McCADConfig.hpp"
  )

# Dependencies
add_subdirectory(3rd_party)

# General library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/general"
  "${PROJECT_SOURCE_DIR}/Implementation/general")
add_subdirectory (Implementation/general)
add_subdirectory (Interface/general)
add_library(INPUTDATA INTERFACE)
target_sources(INPUTDATA INTERFACE
  $<TARGET_OBJECTS:INPUTDATA_IFACE>
  $<TARGET_OBJECTS:INPUTDATA_IMPL>)

# IO library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/io"
  "${PROJECT_SOURCE_DIR}/Implementation/io")
add_subdirectory (Implementation/io)
add_subdirectory (Interface/io)
add_library(IO INTERFACE)
target_sources(IO INTERFACE
  $<TARGET_OBJECTS:IO_IFACE>
  $<TARGET_OBJECTS:IO_IMPL>)

# Tools library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/tools"
  "${PROJECT_SOURCE_DIR}/Implementation/tools")
add_subdirectory (Interface/tools)
add_subdirectory (Implementation/tools)
add_library(TOOLS INTERFACE)
target_sources(TOOLS INTERFACE
  $<TARGET_OBJECTS:TOOLS_IFACE>
  $<TARGET_OBJECTS:TOOLS_IMPL>)

# Geometry library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/geometry"
  "${PROJECT_SOURCE_DIR}/Implementation/geometry")
add_subdirectory (Implementation/geometry)
add_subdirectory (Interface/geometry)
add_library(GEOMETRY INTERFACE)
target_sources(GEOMETRY INTERFACE
  $<TARGET_OBJECTS:GEOMETRY_IFACE>
  $<TARGET_OBJECTS:GEOMETRY_IMPL>)
target_link_libraries(GEOMETRY INTERFACE Coordinates_IMPL TOOLS)

# Decomposition library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/decomposition"
  "${PROJECT_SOURCE_DIR}/Implementation/decomposition")
add_subdirectory (Implementation/decomposition)
add_subdirectory (Interface/decomposition)
add_library(DECOMPOSITION INTERFACE)
target_sources(DECOMPOSITION INTERFACE
  $<TARGET_OBJECTS:DECOMPOSITION_IFACE>
  $<TARGET_OBJECTS:DECOMPOSITION_IMPL>)
target_link_libraries(DECOMPOSITION INTERFACE GEOMETRY)

# Conversion library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/conversion"
  "${PROJECT_SOURCE_DIR}/Implementation/conversion")
add_subdirectory (Implementation/conversion)
add_subdirectory (Interface/conversion)
add_library(CONVERSION INTERFACE)
target_sources(CONVERSION INTERFACE
  $<TARGET_OBJECTS:CONVERSION_IFACE>
  $<TARGET_OBJECTS:CONVERSION_IMPL>)
target_link_libraries(CONVERSION INTERFACE GEOMETRY)

# add the executable
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE
    OCC
    BOOST
    MOAB
    INPUTDATA
    IO
    TOOLS
    GEOMETRY
    DECOMPOSITION
    CONVERSION)

install(TARGETS ${PROJECT_NAME}
  DESTINATION ${PROJECT_BINARY_DIR}/bin)
install(FILES "${PROJECT_BINARY_DIR}/McCADConfig.hpp"
  DESTINATION ${PROJECT_BINARY_DIR}/includes)
