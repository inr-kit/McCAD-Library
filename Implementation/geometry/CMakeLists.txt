add_subdirectory(Coordinates)

add_library(Geometry
    BoundingBox.cpp)

target_sources(Geometry
    PUBLIC
        BoundingBox.hpp
    )

target_link_libraries(Geometry
    PRIVATE
        OCC
    PUBLIC
        Coordinates
    )

target_include_directories(Geometry
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
