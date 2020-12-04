# Coordinates implementation library
message("")
add_subdirectory(common)
add_subdirectory(1D)
add_subdirectory(2D)
add_subdirectory(3D)

#set(LINK_LIBS_INT CoordCommon_IMPL Coord1D_IMPL Coord2D_IMPL Coord3D_IMPL)
#set(LINK_LIBS_DEP "")

#file(GLOB HEADER_FILES "*.hpp" "*.ipp")

#include_directories(${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates
#    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/common
#    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/1D
#    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/2D
#    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/3D
#    ${PROJECT_SOURCE_DIR}/src/tools/Implementation)

#McCAD_build_interface(Coordinates_IMPL)


