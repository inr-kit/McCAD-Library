# IO interface library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT IO_IMPL)
set(LINK_LIBS_DEP)
include_directories(${PROJECT_SOURCE_DIR}/src/io/Interface)
McCAD_build_lib(IO_IFACE)
