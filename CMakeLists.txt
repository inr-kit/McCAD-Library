cmake_minimum_required(VERSION 3.14.4 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(Use_Linux_OS "Linux OS is assumed by default. Change to OFF for Windows" ON)
message("Use_Linux_OS: " ${Use_Linux_OS})

# Set project name
project(McCAD LANGUAGES CXX)

# Set version number
set (McCAD_VERSION_MAJOR 1)
set (McCAD_VERSION_MINOR 0)
set (McCAD_VERSION_RELEASE 0)

# configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/McCADConfig.h.in"
  "${PROJECT_BINARY_DIR}/McCADConfig.hpp"
  )

# add the executable
add_subdirectory(3rd_party)
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE
  OCC_TOOLKITS
  INPUTDATA
  INPUTDATA_IFACE
  IO
  IO_IFACE
  TOOLS
  TOOLS_IFACE
  DECOMPOSITION
  DECOMPOSITION_IFACE)

# General library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/general"
  "${PROJECT_SOURCE_DIR}/Implementation/general")
add_subdirectory (Interface/general)
add_subdirectory (Implementation/general)
target_link_libraries(INPUTDATA INPUTDATA_IFACE OCC_TOOLKITS)

# IO library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/io"
  "${PROJECT_SOURCE_DIR}/Implementation/io")
add_subdirectory (Interface/io)
add_subdirectory (Implementation/io)
target_link_libraries(IO IO_IFACE OCC_TOOLKITS)

# Tools library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/tools"
  "${PROJECT_SOURCE_DIR}/Implementation/tools")
add_subdirectory (Interface/tools)
add_subdirectory (Implementation/tools)
target_link_libraries(TOOLS_IFACE OCC_TOOLKITS)
target_link_libraries(TOOLS TOOLS_IFACE OCC_TOOLKITS)

# Decomposition library
include_directories(
  "${PROJECT_SOURCE_DIR}/Interface/decomposition"
  "${PROJECT_SOURCE_DIR}/Implementation/decomposition")
add_subdirectory (Interface/decomposition)
add_subdirectory (Implementation/decomposition)
target_link_libraries (DECOMPOSITION_IFACE OCC_TOOLKITS)
target_link_libraries (DECOMPOSITION DECOMPOSITION_IFACE TOOLS OCC_TOOLKITS)

install (TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES "${PROJECT_BINARY_DIR}/McCADConfig.hpp" DESTINATION ${PROJECT_BINARY_DIR}/includes)
