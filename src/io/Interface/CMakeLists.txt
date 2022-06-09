# IO interface library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT IO_IMPL INPUTDATA_IFACE)
set(LINK_LIBS_DEP "")
include_directories(${PROJECT_SOURCE_DIR}/src/io/Interface
    ${PROJECT_SOURCE_DIR}/src/io/Implementation
    ${PROJECT_SOURCE_DIR}/src/general/Interface
    ${PROJECT_SOURCE_DIR}/src/general/Implementation
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation
    ${PROJECT_BINARY_DIR}/src/info)
McCAD_build_lib(IO_IFACE)
