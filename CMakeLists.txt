# Project
project(McCAD)
set (McCAD_VERSION_MAJOR 1)
set (McCAD_VERSION_MINOR 1)
set (McCAD_VERSION ${McCAD_VERSION_MAJOR}.${McCAD_VERSION_MINOR})

# CMake
cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
List(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Detect operating system
if(UNIX)
  set(LINUX_OS true)
endif(UNIX)
if(WIN32)
  set(LINUX_OS false)
endif(WIN32)

include(McCAD_build)
# Build options
McCAD_CMake_setup()
# Build directories
McCAD_dir_setup()
# CMake flags
McCAD_CMake_flags()

# Dependencies
find_package(BOOST REQUIRED)
find_package(OCC REQUIRED)

# McCAD
add_subdirectory(src)

# Make config file
message("")
configure_file (${PROJECT_SOURCE_DIR}/src/info/info.hpp.in ${INSTALL_DIR}/src/info/info.hpp)
configure_file (${PROJECT_SOURCE_DIR}/cmake/McCADConfig.cmake.in ${INSTALL_DIR}/McCADConfig.cmake)
install(FILES ${INSTALL_DIR}/McCADConfig.cmake DESTINATION ${INSTALL_LIB_DIR})
install(EXPORT McCADTargets DESTINATION ${INSTALL_LIB_DIR})
message("")
