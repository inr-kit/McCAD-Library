cmake_minimum_required(VERSION 3.14.4 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(Use_Linux_OS "Linux OS is assumed by default. Change to OFF for Windows" OFF)
message(${Use_Linux_OS})

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
  IO
  TOOLS
  DECOMPOSITION
  )

# General library
include_directories ("${PROJECT_SOURCE_DIR}/includes/general")
add_subdirectory (includes/general)
add_subdirectory (sources/general)
target_link_libraries(INPUTDATA OCC_TOOLKITS)

# IO library
include_directories ("${PROJECT_SOURCE_DIR}/includes/io")
add_subdirectory (includes/io)
add_subdirectory (sources/io)
target_link_libraries(IO OCC_TOOLKITS)

# Tools library
include_directories ("${PROJECT_SOURCE_DIR}/includes/tools")
add_subdirectory (includes/tools)
add_subdirectory (sources/tools)
target_link_libraries(TOOLS OCC_TOOLKITS)

# Decomposition library
include_directories ("${PROJECT_SOURCE_DIR}/includes/decomposition")
add_subdirectory (includes/decomposition)
add_subdirectory (sources/decomposition)
target_link_libraries (DECOMPOSITION TOOLS OCC_TOOLKITS)

install (TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES "${PROJECT_BINARY_DIR}/McCADConfig.hpp" DESTINATION ${PROJECT_BINARY_DIR}/includes)
