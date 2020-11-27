# Project
project(McCAD)
set (McCAD_VERSION_MAJOR 1)
set (McCAD_VERSION_MINOR 0)
set (McCAD_VERSION ${McCAD_VERSION_MAJOR}.${McCAD_VERSION_MINOR})

# CMake
cmake_minimum_required(VERSION 3.14.4 FATAL_ERROR)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
List(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Installation directories
set(INSTALL_DIR ${PROJECT_BINARY_DIR})
set(INSTALL_BIN_DIR ${PROJECT_BINARY_DIR}/bin)
set(INSTALL_LIB_DIR ${PROJECT_BINARY_DIR}/lib)
set(INSTALL_INCLUDE_DIR ${PROJECT_BINARY_DIR}/include)

# Build options
message("-- =============")
message("-- Build options")
message("-- =============")
option(Linux_OS "Linux OS is assumed by default. Change to OFF for Windows" ON)
message("-- Linux_OS: " ${Linux_OS})
option(MOAB_DIR "Path to MOAB build directory")
message("-- MOAB_DIR: " ${MOAB_DIR})

# Dependencies
add_subdirectory(dependencies)

# McCAD
add_subdirectory(src)

# Make config file
configure_file (${PROJECT_SOURCE_DIR}/cmake/McCADConfig.cmake.in ${INSTALL_DIR}/McCADConfig.cmake)
install(FILES ${INSTALL_DIR}/McCADConfig.cmake DESTINATION ${INSTALL_LIB_DIR}/cmake)
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_LIB_DIR}/cmake)

# add the executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)
#target_link_libraries(${PROJECT_NAME} PRIVATE
#    BOOST
#    OCC
#    MOAB
#    INPUTDATA
#    IO
#    TOOLS
#    GEOMETRY
#    DECOMPOSITION
#    CONVERSION)
