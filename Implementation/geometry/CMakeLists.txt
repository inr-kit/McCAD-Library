add_subdirectory(Coordinates)

add_library(GEOMETRY
    BoundingBox.cpp)

target_sources(GEOMETRY
    PUBLIC
        BoundingBox.hpp
    )

target_link_libraries(GEOMETRY
    PRIVATE
        OCC_TOOLKITS
    PUBLIC
        Coordinates
    )

target_include_directories(GEOMETRY
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
