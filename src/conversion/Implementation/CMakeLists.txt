# Conversion implementation library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp" "*.tpp")
set(LINK_LIBS_INT IO_IFACE TOOLS_IFACE GEOMETRY_IFACE INPUTDATA_IFACE)
set(LINK_LIBS_DEP OCC_LIBRARIES)
include_directories(${PROJECT_SOURCE_DIR}/src/conversion/Implementation
    ${PROJECT_SOURCE_DIR}/src/conversion/Interface
    ${PROJECT_SOURCE_DIR}/src/io/Interface
    ${PROJECT_SOURCE_DIR}/src/io/Implementation
    ${PROJECT_SOURCE_DIR}/src/general/Interface
    ${PROJECT_SOURCE_DIR}/src/general/Implementation
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation
    ${PROJECT_SOURCE_DIR}/src/tools/Interface
    ${PROJECT_SOURCE_DIR}/src/decomposition/Implementation
    ${PROJECT_SOURCE_DIR}/src/geometry/Interface
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation
    ${PROJECT_BINARY_DIR}/src/info)
McCAD_build_lib(CONVERSION_IMPL)
