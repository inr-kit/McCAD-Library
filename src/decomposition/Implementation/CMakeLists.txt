# Decomposition implementation library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp" "*.tpp")
set(LINK_LIBS_INT TOOLS_IFACE GEOMETRY_IFACE)
set(LINK_LIBS_DEP OCC_LIBRARIES)
include_directories(${PROJECT_SOURCE_DIR}/src/decomposition/Implementation
    ${PROJECT_SOURCE_DIR}/src/decomposition/Interface
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation
    ${PROJECT_SOURCE_DIR}/src/geometry/Interface
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation
    ${PROJECT_SOURCE_DIR}/src/tools/Interface
    ${PROJECT_SOURCE_DIR}/src/general/Implementation
    ${PROJECT_SOURCE_DIR}/src/general/Interface)
McCAD_build_lib(DECOMPOSITION_IMPL)
