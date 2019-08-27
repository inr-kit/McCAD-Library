add_library(Coord2D
    Coord2D.cpp
    Coord2D_const.cpp
    Coord2D_hash.cpp
    Coord2D_util.cpp
    )

target_sources(Coord2D
    PUBLIC
        Coord2D.fwd
        Coord2D.hpp
        Coord2D_const.hpp
        Coord2D_hash.hpp
        Coord2D_util.hpp
        Coord2D.ipp
    )

target_link_libraries(Coord2D
    PRIVATE
        FloatingPoint
        CoordCommon
        Coord1D
    )

target_include_directories(Coord2D
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

