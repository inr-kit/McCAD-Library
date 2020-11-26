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
set(CMAKE_SHARED_LIBRARY_SUFFIX ".a")
List(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Execute options
message("-- =======================")
message("-- McCAD${McCAD_VERSION}L build options")
message("-- =======================")
option(Linux_OS "Linux OS is assumed by default. Change to OFF for Windows" ON)
message("-- Linux_OS: " ${Linux_OS})
option(MOAB_DIR "Path to MOAB build directory")
message("-- MOAB_DIR: " ${MOAB_DIR})

# Configure a header file to pass some of the CMake settings to the source code
configure_file (${PROJECT_SOURCE_DIR}/cmake/McCADConfig.h.in
                ${PROJECT_BINARY_DIR}/McCADConfig.cmake)

# Dependencies
add_subdirectory(dependencies)

# McCAD
add_subdirectory(src)

# add the executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)

install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_BINARY_DIR}/bin)
install(FILES ${PROJECT_BINARY_DIR}/McCADConfig.cmake
        DESTINATION ${PROJECT_BINARY_DIR}/includes)
target_link_libraries(${PROJECT_NAME} PRIVATE
    BOOST
    OCC
    MOAB
    INPUTDATA
    IO
    TOOLS
    GEOMETRY
    DECOMPOSITION
    CONVERSION)
