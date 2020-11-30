# TOOLS implementation library
message("")
add_subdirectory(FloatingPoint)

file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")
set(LINK_LIBS_INT "")
set(LINK_LIBS_DEP OCC_LIBRARIES BOOST_LIBRARIES)
include_directories(${PROJECT_SOURCE_DIR}/src/tools/Implementation
                    ${PROJECT_SOURCE_DIR}/src/tools/Interface)
McCAD_build_lib(TOOLS_IMPL)
