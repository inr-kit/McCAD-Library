# Coord2D implementation library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp" "*.ipp")
set(LINK_LIBS_INT CoordCommon_IMPL Coord1D_IMPL)
set(LINK_LIBS_DEP "")
include_directories(${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/2D
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/1D
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/common
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation/FloatingPoint)
McCAD_build_lib(Coord2D_IMPL)
