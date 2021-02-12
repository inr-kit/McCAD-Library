macro (McCAD_CMake_setup)
  message("")
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
  endif ()
  if (NOT CMAKE_BUILD_TYPE STREQUAL "Release" AND
      NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND
      NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(FATAL_ERROR "Specified CMAKE_BUILD_TYPE is not supported; please choose from [Release, Debug, RelWithDebInfo]")
  endif ()
  if (NOT BUILD_SHARED AND NOT BUILD_STATIC)
    message(FATAL_ERROR "BUILD_SHARED and BUILD_STATIC are both OFF!")
  endif ()
  message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
  message(STATUS "CMAKE_STATIC_LIBRARY_SUFFIX: " ${CMAKE_STATIC_LIBRARY_SUFFIX})
  message(STATUS "CMAKE_SHARED_LIBRARY_SUFFIX: " ${CMAKE_SHARED_LIBRARY_SUFFIX})
endmacro ()

macro (McCAD_dir_setup)
  message("")
  set(INSTALL_DIR ${PROJECT_BINARY_DIR})
  set(INSTALL_BIN_DIR "${PROJECT_BINARY_DIR}/bin")
  set(INSTALL_LIB_DIR "${PROJECT_BINARY_DIR}/lib")
  set(INSTALL_INCLUDE_DIR "${PROJECT_BINARY_DIR}/include")
  set(CMAKE_INSTALL_PREFIX ${INSTALL_DIR})
endmacro ()

macro (McCAD_CMake_flags)
  message("")
  if (BUILD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  endif ()

  if (BUILD_STATIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_Fortran_FLAGS)
    set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)
    set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
    set(CMAKE_EXE_LINK_DYNAMIC_Fortran_FLAGS)
  else ()
    #set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif ()

  message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
  message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
  message(STATUS "CMAKE_Fortran_FLAGS: ${CMAKE_Fortran_FLAGS}")
  message(STATUS "CMAKE_C_IMPLICIT_LINK_LIBRARIES: ${CMAKE_C_IMPLICIT_LINK_LIBRARIES}")
  message(STATUS "CMAKE_CXX_IMPLICIT_LINK_LIBRARIES: ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES}")
  message(STATUS "CMAKE_Fortran_IMPLICIT_LINK_LIBRARIES: ${CMAKE_Fortran_IMPLICIT_LINK_LIBRARIES}")
  message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
endmacro ()

# Set link libraries
macro (McCAD_link_lib)
    # Find required libraries to link to
    set(LINK_SHARED_LIBS)
    set(LINK_STATIC_LIBS)
    # Append dependency libraries to list
    foreach (dep_lib IN LISTS LINK_LIBS_DEP)
        list(APPEND LINK_SHARED_LIBS ${${dep_lib}_SHARED})
        list(APPEND LINK_STATIC_LIBS ${${dep_lib}_STATIC})
    endforeach ()
    # Append internal "McCAD" libraries to list
    foreach (int_lib IN LISTS LINK_LIBS_INT)
        list(APPEND LINK_SHARED_LIBS ${int_lib}-shared)
        list(APPEND LINK_STATIC_LIBS ${int_lib}-static)
    endforeach ()
    if (BUILD_SHARED)
        message(STATUS "LINK_SHARED_LIBS:" ${LINK_SHARED_LIBS})
    endif()
    if(BUILD_STATIC)
        message(STATUS "LINK_STATIC_LIBS:" ${LINK_STATIC_LIBS})
    endif ()
endmacro ()

# Build object libraries
macro (McCAD_build_object lib)
    message(STATUS "Building library: ${lib}")
    McCAD_link_lib()
    # Build shared libraries
    if (BUILD_SHARED)
        add_library(${lib}-shared OBJECT ${SRC_FILES})
        target_link_libraries(${lib}-shared PUBLIC ${LINK_SHARED_LIBS})
        install(FILES ${HEADER_FILES}
            EXPORT McCADTargets
            DESTINATION ${INSTALL_LIB_DIR})
    endif ()
    # Build static libraries
    if (BUILD_STATIC)
        add_library(${lib}-static OBJECT ${SRC_FILES})
        target_link_libraries(${lib}-static ${LINK_STATIC_LIBS})
        install(FILES ${HEADER_FILES}
            EXPORT McCADTargets
            DESTINATION ${INSTALL_LIB_DIR})
    endif ()
endmacro ()

# Build object libraries
macro (McCAD_build_object_top lib)
    message(STATUS "Building library: ${lib}")
    McCAD_link_lib()
    # Build shared libraries
    if (BUILD_SHARED)
        add_library(${lib}-shared OBJECT)
        target_link_libraries(${lib}-shared PUBLIC ${LINK_SHARED_LIBS})
        install(FILES ${HEADER_FILES}
            EXPORT McCADTargets
            DESTINATION ${INSTALL_LIB_DIR})
    endif ()
    # Build static libraries
    if (BUILD_STATIC)
        add_library(${lib}-static OBJECT)
        target_link_libraries(${lib}-static ${LINK_STATIC_LIBS})
        install(FILES ${HEADER_FILES}
            EXPORT McCADTargets
            DESTINATION ${INSTALL_LIB_DIR})
    endif ()
endmacro ()

# Build interface libraries
macro (McCAD_build_interface lib)
    message(STATUS "Building library: ${lib}")
    # Build shared libraries
    if (BUILD_SHARED)
        add_library(${lib}-shared INTERFACE)
        install(FILES ${HEADER_FILES}
            EXPORT McCADTargets
            DESTINATION ${INSTALL_LIB_DIR})
    endif()
    # Build static libraries
    if (BUILD_STATIC)
        add_library(${lib}-static INTERFACE)
        install(FILES ${HEADER_FILES}
            EXPORT McCADTargets
            DESTINATION ${INSTALL_LIB_DIR})
    endif()
endmacro ()

# Build libraries
macro (McCAD_build_lib lib)
    message(STATUS "Building library: ${lib}")
    McCAD_link_lib()
    # Build shared libraries
    if (BUILD_SHARED)
        add_library(${lib}-shared SHARED ${SRC_FILES})
        set_target_properties(${lib}-shared PROPERTIES OUTPUT_NAME ${lib} PUBLIC_HEADER "${HEADER_FILES}")
        target_link_libraries(${lib}-shared  PUBLIC ${LINK_SHARED_LIBS})
        #target_include_directories(${lib}-shared INTERFACE $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>)
        install(TARGETS ${lib}-shared
            EXPORT McCADTargets
            LIBRARY DESTINATION ${INSTALL_LIB_DIR}
            PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR})
    endif ()
    # Build static libraries
    if (BUILD_STATIC)
        add_library(${lib}-static STATIC ${SRC_FILES})
        set_target_properties(${lib}-static PROPERTIES OUTPUT_NAME ${lib})
        target_link_libraries(${lib}-static ${LINK_STATIC_LIBS})
        #target_include_directories(${lib}-static INTERFACE $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>)
        install(TARGETS ${lib}-static
            EXPORT McCADTargets
            LIBRARY DESTINATION ${INSTALL_LIB_DIR}
            PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR})
    endif ()
endmacro ()

# Install an executable
macro (McCAD_install_exe exe)
    message(STATUS "Building executable: ${exe}")
    McCAD_link_lib()
    add_executable(${exe} ${SRC_FILES})
    if (BUILD_STATIC)
        target_link_libraries(${exe} ${LINK_STATIC_LIBS})
    else ()
        target_link_libraries(${exe} PUBLIC ${LINK_SHARED_LIBS})
    endif ()
    install(TARGETS ${exe} DESTINATION ${INSTALL_BIN_DIR})
endmacro ()
