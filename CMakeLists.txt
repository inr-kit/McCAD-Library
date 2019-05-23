cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set project name
project(McCAD LANGUAGES CXX)

# Set version number
set (McCAD_VERSION_MAJOR 1)
set (McCAD_VERSION_MINOR 0)
set (McCAD_VERSION_RELEASE 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/McCADConfig.h.in"
  "${PROJECT_BINARY_DIR}/McCADConfig.hpp"
  )

# add the executable
add_executable(${PROJECT_NAME} "main.cpp")

# add the binary tree to the search path for include files
# so that we will find McCADConfig.h
target_include_directories(McCAD PUBLIC "${PROJECT_BINARY_DIR}")

# Optional SolidsCollection library
option (SolidsCollection "Use provided SolidsCollection implementation" ON)
if (SolidsCollection)
  include_directories ("${PROJECT_SOURCE_DIR}/source/universal")
  add_subdirectory (source/universal)
  set (EXTRA_LIBS ${EXTRA_LIBS} SolidsCollection)
endif (SolidsCollection)
target_link_libraries(McCAD ${EXTRA_LIBS})

# Decomposition library
include_directories ("${PROJECT_SOURCE_DIR}/source/decomposition")
add_subdirectory (source/decomposition)
set (EXTRA_LIBS ${EXTRA_LIBS} Decomposition)
target_link_libraries(McCAD ${EXTRA_LIBS})

# Void Generation library
include_directories ("${PROJECT_SOURCE_DIR}/source/void")
add_subdirectory (source/void)
set (EXTRA_LIBS ${EXTRA_LIBS} Void)
target_link_libraries(McCAD ${EXTRA_LIBS})

# Materials library
include_directories ("${PROJECT_SOURCE_DIR}/source/materials")
add_subdirectory (source/materials)
set (EXTRA_LIBS ${EXTRA_LIBS} Materials)
target_link_libraries(McCAD ${EXTRA_LIBS})

install (TARGETS McCAD DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES "${PROJECT_BINARY_DIR}/McCADConfig.hpp" DESTINATION ${PROJECT_BINARY_DIR}/include)
