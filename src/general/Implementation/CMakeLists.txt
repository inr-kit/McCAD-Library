# General implementation library
message("")
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT)
set(LINK_LIBS_DEP OCC_LIBRARIES)
include_directories(${PROJECT_SOURCE_DIR}/src/general/Implementation
                    ${PROJECT_SOURCE_DIR}/src/general/Interface
                    ${OCC_INCLUDE_DIRS})
McCAD_build_lib(INPUTDATA_IMPL)
