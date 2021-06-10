# Geometry implementation library
message("")
add_subdirectory(Coordinates)

file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT TOOLS_IMPL CoordCommon_IMPL Coord1D_IMPL Coord2D_IMPL Coord3D_IMPL)
set(LINK_LIBS_DEP OCC_LIBRARIES)
include_directories(${PROJECT_SOURCE_DIR}/src/geometry/Implementation
    ${PROJECT_SOURCE_DIR}/src/tools/Interface
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/common
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/1D
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/2D
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/3D
    ${PROJECT_SOURCE_DIR}/src/decomposition/Implementation
    ${PROJECT_SOURCE_DIR}/src/geometry/Interface)
McCAD_build_lib(GEOMETRY_IMPL)
