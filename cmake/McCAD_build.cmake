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
        set_target_properties(${lib}-static PROPERTIES OUTPUT_NAME ${lib} PUBLIC_HEADER "${HEADER_FILES}")
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
