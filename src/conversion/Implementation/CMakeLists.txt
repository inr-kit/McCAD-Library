# Conversion implementation library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT TOOLS_IFACE IO_IMPL INPUTDATA_IFACE GEOMETRY_IMPL)
set(LINK_LIBS_DEP OCC_LIBRARIES) #MOAB_LIBRARIES
include_directories(${PROJECT_SOURCE_DIR}/src/conversion/Implementation
    ${PROJECT_SOURCE_DIR}/src/general/Interface
    ${PROJECT_SOURCE_DIR}/src/general/Implementation
    ${PROJECT_SOURCE_DIR}/src/io/Interface
    ${PROJECT_SOURCE_DIR}/src/io/Implementation
    ${PROJECT_SOURCE_DIR}/src/conversion/Interface
    ${PROJECT_SOURCE_DIR}/src/tools/Interface
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation
    ${PROJECT_SOURCE_DIR}/src/decomposition/Implementation
    ${PROJECT_SOURCE_DIR}/src/geometry/Interface
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation)
McCAD_build_lib(CONVERSION_IMPL)
