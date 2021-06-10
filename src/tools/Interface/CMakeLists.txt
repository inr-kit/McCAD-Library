# TOOLS interface library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT TOOLS_IMPL)
set(LINK_LIBS_DEP "")
include_directories(${PROJECT_SOURCE_DIR}/src/tools/Interface
    ${PROJECT_SOURCE_DIR}/src/tools/Implementation)
McCAD_build_lib(TOOLS_IFACE)

