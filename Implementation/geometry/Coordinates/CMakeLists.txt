add_subdirectory(common)
add_subdirectory(1D)
add_subdirectory(2D)
add_subdirectory(3D)

add_library(Coordinates INTERFACE)

target_link_libraries(Coordinates
    INTERFACE
        CoordCommon
        Coord1D
        Coord2D
        Coord3D
    )

target_include_directories(Coordinates
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

