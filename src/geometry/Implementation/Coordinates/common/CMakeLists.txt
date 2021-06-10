# CoordCommon implementation library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp" "*.ipp")
set(LINK_LIBS_INT TOOLS_IMPL)
set(LINK_LIBS_DEP "")

include_directories(${PROJECT_SOURCE_DIR}/src/geometry/Implementation/Coordinates/common
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation)

McCAD_build_lib(CoordCommon_IMPL)
