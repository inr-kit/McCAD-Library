# Conversion implementation library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT CONVERSION_IMPL INPUTDATA_IFACE)
set(LINK_LIBS_DEP "")
include_directories(${PROJECT_SOURCE_DIR}/src/conversion/Interface
    ${PROJECT_SOURCE_DIR}/src/conversion/Implementation
    ${PROJECT_SOURCE_DIR}/src/general/Interface
    ${PROJECT_SOURCE_DIR}/src/io/Interface
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation
    ${PROJECT_SOURCE_DIR}/src/tools/Interface
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation
    ${PROJECT_SOURCE_DIR}/src/geometry/Interface)
McCAD_build_lib(CONVERSION_IFACE)
