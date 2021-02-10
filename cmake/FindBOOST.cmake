message("")
message("-- ==================")
message("-- Locating BOOST C++")
message("-- ==================")

if(Linux_OS)
    find_package(Boost 1.71 REQUIRED)
    if(Boost_FOUND AND NOT DEFINED BOOST_INCLUDE_DIRS)
        set(BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
    endif(Boost_FOUND AND NOT DEFINED BOOST_INCLUDE_DIRS)
    if(Boost_FOUND AND NOT DEFINED BOOST_LIBRARY_DIRS)
        set(BOOST_LIBRARY_DIRS ${Boost_INCLUDE_DIRS}/../lib)
    endif(Boost_FOUND AND NOT DEFINED BOOST_LIBRARY_DIRS)

    if(Boost_FOUND)
        if(BUILD_SHARED)
            set(BOOST_LIBRARIES_SHARED "")
            set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX})
            foreach (Boost_lib ${Boost_LIBRARIES})
                set(BOOST_LIB "BOOST_LIB-NOTFOUND")
                find_library(BOOST_LIB
                    NAMES ${Boost_lib}
                    HINTS ${BOOST_LIBRARY_DIRS}
                    NO_DEFAULT_PATH)
                list(APPEND BOOST_LIBRARIES_SHARED ${BOOST_LIB})
            endforeach ()
        endif()
        if(BUILD_STATIC)
            set(BOOST_LIBRARIES_STATIC "")
            set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
            foreach (Boost_lib ${Boost_LIBRARIES})
                set(BOOST_LIB "BOOST_LIB-NOTFOUND")
                find_library(BOOST_LIB
                    NAMES ${Boost_lib}
                    HINTS ${BOOST_LIBRARY_DIRS}
                    NO_DEFAULT_PATH)
                list(APPEND BOOST_LIBRARIES_STATIC ${BOOST_LIB})
            endforeach ()
        endif()
    endif()
else(Linux_OS)
    if(DEFINED BOOST_ROOT)
        set(Boost_FOUND True)
    endif(DEFINED BOOST_ROOT)

    if(Boost_FOUND AND NOT DEFINED BOOST_INCLUDE_DIRS)
        set(BOOST_INCLUDE_DIRS ${BOOST_ROOT})
    endif(Boost_FOUND AND NOT DEFINED BOOST_INCLUDE_DIRS)
    if(Boost_FOUND AND NOT DEFINED BOOST_LIBRARY_DIRS)
        set(BOOST_LIBRARY_DIRS ${BOOST_ROOT}/libs)
    endif(Boost_FOUND AND NOT DEFINED BOOST_LIBRARY_DIRS)
endif()

message(STATUS "BOOST_INCLUDE_DIRS: ${BOOST_INCLUDE_DIRS}")
message(STATUS "BOOST_LIBRARY_DIRS: ${BOOST_LIBRARY_DIRS}")
if (BUILD_SHARED)
    message(STATUS "BOOST_LIBRARIES_SHARED: ${BOOST_LIBRARIES_SHARED}")
else()
    message(STATUS "BOOST_LIBRARIES_STATIC: ${BOOST_LIBRARIES_STATIC}")
endif()
include_directories(${BOOST_INCLUDE_DIRS})
message("")

