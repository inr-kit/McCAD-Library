# Decomposition iinterface library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT DECOMPOSITION_IMPL)
set(LINK_LIBS_DEP)
include_directories(${PROJECT_SOURCE_DIR}/src/decomposition/Interface
    ${PROJECT_SOURCE_DIR}/src/decomposition/Implementation)
McCAD_build_lib(DECOMPOSITION_IFACE)
