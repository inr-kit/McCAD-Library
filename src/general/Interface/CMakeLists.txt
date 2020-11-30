# IO interface library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT INPUTDATA_IMPL)
set(LINK_LIBS_DEP)
include_directories(${PROJECT_SOURCE_DIR}/src/general/Interface
                    ${PROJECT_SOURCE_DIR}/src/general/Implementation)
McCAD_build_lib(INPUTDATA_IFACE)
