cmake_minimum_required(VERSION 3.14.4 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(Linux_OS "Linux OS is assumed by default. Change to OFF for Windows"ON)

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
target_link_libraries(${PROJECT_NAME} OCC_TOOLKITS)

# General library
include_directories ("${PROJECT_SOURCE_DIR}/includes/general")
add_subdirectory (includes/general)
add_subdirectory (sources/general)
set (EXTRA_LIBS ${EXTRA_LIBS} InputData InputData_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# IO library
include_directories ("${PROJECT_SOURCE_DIR}/includes/io")
add_subdirectory (includes/io)
add_subdirectory (sources/io)
set (EXTRA_LIBS ${EXTRA_LIBS} STEPReader STEPReader_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
set (EXTRA_LIBS ${EXTRA_LIBS} STEPWriter STEPWriter_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# Tools library
include_directories ("${PROJECT_SOURCE_DIR}/includes/tools")
add_subdirectory (includes/tools)
add_subdirectory (sources/tools)
set (EXTRA_LIBS ${EXTRA_LIBS} Tools Tools_impl)
target_link_libraries(Tools_impl OCC_TOOLKITS)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# Surface library
include_directories ("${PROJECT_SOURCE_DIR}/includes/decomposition")
add_subdirectory (includes/decomposition)
add_subdirectory (sources/decomposition)
target_link_libraries(Surface Tools OCC_TOOLKITS)
target_link_libraries(Surface_impl Tools_impl)
set (EXTRA_LIBS ${EXTRA_LIBS} Surface Surface_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# Edge library
set (EXTRA_LIBS ${EXTRA_LIBS} Edge Edge_impl)
target_link_libraries(Edge Tools)
target_link_libraries(Edge_impl Tools_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# MeshTriangle library
set (EXTRA_LIBS ${EXTRA_LIBS} Meshtriangle Meshtriangle_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# BoundSurface library
set (EXTRA_LIBS ${EXTRA_LIBS} BoundSurface BoundSurface_impl)
target_link_libraries(BoundSurface Surface)
target_link_libraries(BoundSurface_impl Surface_impl Meshtriangle Meshtriangle_impl OCC_TOOLKITS)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# BoundSurfacePlane library
set (EXTRA_LIBS ${EXTRA_LIBS} BoundSurfacePlane BoundSurfacePlane_impl)
target_link_libraries(BoundSurfacePlane BoundSurface)
target_link_libraries(BoundSurfacePlane_impl BoundSurface_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# SplitSurfaces library
set (EXTRA_LIBS ${EXTRA_LIBS} SplitSurfaces SplitSurfaces_impl)
target_link_libraries(SplitSurfaces SplitSurfaces_impl OCC_TOOLKITS)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# SplitSolid library
set (EXTRA_LIBS ${EXTRA_LIBS} SplitSolid SplitSolid_impl)
target_link_libraries(SplitSolid_impl OCC_TOOLKITS)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# DecomposeSolid library
set (EXTRA_LIBS ${EXTRA_LIBS} DecomposeSolid DecomposeSolid_impl)
target_link_libraries(DecomposeSolid_impl Solid Solid_impl SplitSurfaces
  SplitSurfaces_impl SplitSolid SplitSolid_impl Edge Edge_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# Decomposition library
set (EXTRA_LIBS ${EXTRA_LIBS} Decomposition Decomposition_impl)
target_link_libraries(Decomposition Tools DecomposeSolid)
target_link_libraries(Decomposition_impl DecomposeSolid_impl Tools_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# Solid library
set (EXTRA_LIBS ${EXTRA_LIBS} Solid Solid_impl)
target_link_libraries(Solid BoundSurfacePlane)
target_link_libraries(Solid_impl BoundSurfacePlane_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

install (TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES "${PROJECT_BINARY_DIR}/McCADConfig.hpp" DESTINATION ${PROJECT_BINARY_DIR}/includes)
