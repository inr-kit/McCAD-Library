add_library(CoordCommon
    coord_type_const.cpp
    coord_type_util.cpp
    )

target_sources(CoordCommon
    PUBLIC
        coord_type.hpp
        coord_type_const.hpp
        coord_type_util.hpp
        Dimension.hpp
    )

target_link_libraries(CoordCommon
    PRIVATE
        FloatingPoint
    )

target_include_directories(CoordCommon
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

