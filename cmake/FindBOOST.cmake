message("")
message("-- ============================")
message("-- Locating BOOST C++ Libraries")
message("-- ============================")

if(LINUX_OS)
    if(BOOST_CUSTOM_ROOT)
        set(Boost_FOUND True)
        if(Boost_FOUND AND NOT DEFINED BOOST_INCLUDE_DIRS)
            set(BOOST_INCLUDE_DIRS "${BOOST_CUSTOM_ROOT}")
            include_directories(${BOOST_INCLUDE_DIRS})
        endif()
        if(Boost_FOUND AND NOT DEFINED BOOST_LIBRARY_DIRS)
            set(BOOST_LIBRARY_DIRS "${BOOST_CUSTOM_ROOT}/libs")
        endif()
    else()
        find_package(Boost 1.78 REQUIRED)
        if(Boost_FOUND AND NOT DEFINED BOOST_INCLUDE_DIRS)
            set(BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
            include_directories(${BOOST_INCLUDE_DIRS})
        endif()
        if(Boost_FOUND AND NOT DEFINED BOOST_LIBRARY_DIRS)
            set(BOOST_LIBRARY_DIRS "${Boost_INCLUDE_DIRS}/../lib")
        endif()
    endif()

else(LINUX_OS)
    if(DEFINED BOOST_CUSTOM_ROOT)
        set(Boost_FOUND True)
    else()
        message(FATAL_ERROR "BOOST_CUSTOM_PATH is not defined!")
    endif()

    if(Boost_FOUND AND NOT DEFINED BOOST_INCLUDE_DIRS)
        set(BOOST_INCLUDE_DIRS ${BOOST_CUSTOM_ROOT})
        include_directories(${BOOST_INCLUDE_DIRS})
    endif()
    if(Boost_FOUND AND NOT DEFINED BOOST_LIBRARY_DIRS)
        set(BOOST_LIBRARY_DIRS "${BOOST_CUSTOM_ROOT}/libs")
    endif()
endif()

message(STATUS "BOOST_INCLUDE_DIRS: ${BOOST_INCLUDE_DIRS}")
message(STATUS "BOOST_LIBRARY_DIRS: ${BOOST_LIBRARY_DIRS}")
message("")

