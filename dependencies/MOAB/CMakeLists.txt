if(Linux_OS)
    find_package(MOAB REQUIRED)

    if(MOAB_FOUND AND DEFINED MOAB_LIBRARY_DIRS AND DEFINED MOAB_INCLUDE_DIRS)
        add_library(MOAB INTERFACE)
        install (TARGETS MOAB DESTINATION ${INSTALL_LIB_DIR})
        target_link_directories(MOAB INTERFACE ${MOAB_LIBRARY_DIRS})
        target_include_directories(MOAB INTERFACE ${MOAB_INCLUDE_DIRS})
        target_link_libraries(MOAB INTERFACE ${MOAB_LIBRARIES_STATIC})
    endif()
endif(Linux_OS)


