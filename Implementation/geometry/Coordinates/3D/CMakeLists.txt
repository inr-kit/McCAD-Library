add_library(Coord3D
    Coord3D.cpp
    Coord3D_const.cpp
    Coord3D_hash.cpp
    Coord3D_util.cpp
    )

target_sources(Coord3D
    PUBLIC
        Coord3D.fwd
        Coord3D.hpp
        Coord3D_const.hpp
        Coord3D_hash.hpp
        Coord3D_util.hpp
        Coord3D.ipp
    )

target_link_libraries(Coord3D
    PRIVATE
        FloatingPoint
        CoordCommon
        Coord1D
    )

target_include_directories(Coord3D
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

