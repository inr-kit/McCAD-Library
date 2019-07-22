cmake_minimum_required(VERSION 3.14.4 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# ============================================================
# This part is imported from https://github.com/tpaviot/oce/blob/master/examples/find_package_components/CMakeLists.txt
# find_package(OCE) by specifying a list of toolkits.
# If a toolkit is missing or if OCE is not found, try to find
# an OpenCascade installation.
set(OCE_TOOLKITS FWOSPlugin TKCDF TKLCAF TKPShape TKSTL TKXMesh PTKernel TKernel TKMath TKService TKTObj TKXml TKBin TKFeat TKMesh TKShapeSchema TKTopAlgo TKXmlL TKBinL TKFillet TKMeshVS TKShHealing TKV3d TKXmlTObj TKBinTObj TKG2d TKNIS TKStdLSchema TKVoxel TKXmlXCAF TKBinXCAF TKG3d TKOffset TKStdSchema TKVRML TKXSBase TKBO TKGeomAlgo TKOpenGl TKSTEP TKXCAF TKBool TKGeomBase TKPCAF TKSTEP209 TKXCAFSchema TKBRep TKHLR TKPLCAF TKSTEPAttr TKXDEIGES TKCAF TKIGES TKPrim TKSTEPBase TKXDESTEP)
find_package(OCE COMPONENTS ${OCE_TOOLKITS})
if(OCE_FOUND)
  message(STATUS "Found OCE version ${OCE_VERSION}")
  if(NOT OCE_ALL_FOUND)
    set(OCE_FOUND false)
    message(WARNING "Ignoring OCE installation due to missing toolkit(s): ${OCE_MISSING_TOOLKITS}")
  endif(NOT OCE_ALL_FOUND)
endif(OCE_FOUND)

if(OCE_FOUND)
  # Include files reside in ${OCE_INCLUDE_DIRS};
  include_directories(${OCE_INCLUDE_DIRS})
  message(STATUS "  > OCE include_dir ${OCE_INCLUDE_DIRS}")
  # We do not need library path, they will be automatically imported.
else(OCE_FOUND)
  # OCE not found; either it is not found and user
  # has to set OCE_DIR to the directory containing
  # OCEConfig.cmake, or OCE is not installed and we
  # try to find OpenCascade files.
  if(DEFINED ENV{CASROOT})
    if(NOT DEFINED OCC_INCLUDE_PATH)
      set(OCC_INCLUDE_PATH "$ENV{CASROOT}/inc")
    endif(NOT DEFINED OCC_INCLUDE_PATH)
    if(NOT DEFINED OCC_LIB_PATH)
      if(WIN32)
	set(OCC_LIB_PATH "$ENV{CASROOT}/win32/lib")
      else(WIN32)
	if(APPLE)
	  set(OCC_LIB_PATH "/Library/OpenCASCADE/6.3.0/lib")
	else(APPLE)
	  set(OCC_LIB_PATH "$ENV{CASROOT}/lin/lib")
	endif(APPLE)
      endif(WIN32)
    endif(NOT DEFINED OCC_LIB_PATH)
  else(DEFINED ENV{CASROOT})
    if(NOT DEFINED OCC_INCLUDE_PATH OR NOT DEFINED OCC_LIB_PATH)
      message(WARNING "To specify paths of OpenCascade files, you may either define the CASROOT environment variable, or set both OCC_INCLUDE_PATH and OCC_LIB_PATH variables.")
    endif(NOT DEFINED OCC_INCLUDE_PATH OR NOT DEFINED OCC_LIB_PATH)
  endif(DEFINED ENV{CASROOT})
  if(DEFINED OCC_INCLUDE_PATH)
    message(STATUS "OCC search path for include files: OCC_INCLUDE_PATH=${OCC_INCLUDE_PATH}")
    include_directories(${OCC_INCLUDE_PATH})
  endif(DEFINED OCC_INCLUDE_PATH)
  if(DEFINED OCC_LIB_PATH)
    message(STATUS "OCC search path for libraries: OCC_LIB_PATH=${OCC_LIB_PATH}")
    link_directories(${OCC_LIB_PATH})
  endif(DEFINED OCC_LIB_PATH)
endif(OCE_FOUND)
# =========================================================

# add the executable
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} ${OCE_TOOLKITS})

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

# Tools library
include_directories ("${PROJECT_SOURCE_DIR}/includes/tools")
add_subdirectory (includes/tools)
add_subdirectory (sources/tools)
set (EXTRA_LIBS ${EXTRA_LIBS} Tools Tools_impl)
target_link_libraries(Tools_impl TKShHealing)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})


# Surface library
include_directories ("${PROJECT_SOURCE_DIR}/includes/decomposition")
add_subdirectory (includes/decomposition)
add_subdirectory (sources/decomposition)
set (EXTRA_LIBS ${EXTRA_LIBS} Surface Surface_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# Edge library
set (EXTRA_LIBS ${EXTRA_LIBS} Edge Edge_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# MeshTriangle library
set (EXTRA_LIBS ${EXTRA_LIBS} Meshtriangle Meshtriangle_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# BoundSurface library
set (EXTRA_LIBS ${EXTRA_LIBS} BoundSurface BoundSurface_impl)
target_link_libraries(BoundSurface Surface)
target_link_libraries(BoundSurface_impl Surface_impl)
target_link_libraries(BoundSurface_impl TKMesh)
target_link_libraries(BoundSurface_impl Meshtriangle)
target_link_libraries(BoundSurface_impl Meshtriangle_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# BoundSurfacePlane library
set (EXTRA_LIBS ${EXTRA_LIBS} BoundSurfacePlane BoundSurfacePlane_impl)
target_link_libraries(BoundSurfacePlane BoundSurface)
target_link_libraries(BoundSurfacePlane_impl BoundSurface_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})


# DecomposeSolid library
set (EXTRA_LIBS ${EXTRA_LIBS} DecomposeSolid DecomposeSolid_impl)
target_link_libraries(DecomposeSolid_impl BoundSurfacePlane)
target_link_libraries(DecomposeSolid_impl BoundSurfacePlane_impl)
target_link_libraries(DecomposeSolid_impl Edge)
target_link_libraries(DecomposeSolid_impl Edge_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})


# Decomposition library
set (EXTRA_LIBS ${EXTRA_LIBS} Decomposition Decomposition_impl)
target_link_libraries(Decomposition Tools)
target_link_libraries(Decomposition_impl Tools_impl)
target_link_libraries(Decomposition DecomposeSolid)
target_link_libraries(Decomposition_impl DecomposeSolid_impl)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})


install (TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES "${PROJECT_BINARY_DIR}/McCADConfig.hpp" DESTINATION ${PROJECT_BINARY_DIR}/includes)
