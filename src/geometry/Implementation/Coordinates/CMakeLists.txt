# Coordinates implementation library
message("")
add_subdirectory(common)
add_subdirectory(1D)
add_subdirectory(2D)
add_subdirectory(3D)

file(GLOB HEADER_FILES "*.ipp")
set(LINK_LIBS_INT FloatingPoint_IMPL)
set(LINK_LIBS_DEP "")
include_directories(${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates
    ${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates/common
    ${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates/1D
    ${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates/2D
    ${PROJECT_SOURCE_DIR}/src/Implementation/geometry/Coordinates/3D)
McCAD_build_interface(Coordinates_IMPL)
