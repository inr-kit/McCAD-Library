# Decomposition iinterface library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT DECOMPOSITION_IMPL)
set(LINK_LIBS_DEP "")
include_directories(${PROJECT_SOURCE_DIR}/src/decomposition/Interface
    ${PROJECT_SOURCE_DIR}/src/decomposition/Implementation
    ${PROJECT_SOURCE_DIR}/src/tools/Interface
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation
    ${PROJECT_SOURCE_DIR}/src/geometry/Interface
    ${PROJECT_SOURCE_DIR}/src/geometry/Implementation
    ${PROJECT_SOURCE_DIR}/src/general/Interface
    ${PROJECT_SOURCE_DIR}/src/general/Implementation)
McCAD_build_lib(DECOMPOSITION_IFACE)
