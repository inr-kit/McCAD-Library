add_library(Coord1D
    Coord.cpp
    Coord_const.cpp
    Coord_hash.cpp
    Coord_util.cpp
    )

target_sources(Coord1D
    PUBLIC
        Coord.fwd
        Coord.hpp
        Coord_const.hpp
        Coord_hash.hpp
        Coord_util.hpp
        Coord.ipp
    )

target_link_libraries(Coord1D
    PRIVATE
        CoordCommon
        FloatingPoint
    )

target_include_directories(Coord1D
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
