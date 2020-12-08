# FloatingPoint implementation library
message("")
file(GLOB HEADER_FILES "*.hpp" "*.tpp")
set(LINK_LIBS_INT "")
set(LINK_LIBS_DEP "")
include_directories(${PROJECT_SOURCE_DIR}/src/tools/Implementation/FloatingPoint)
McCAD_build_interface(FloatingPoint_IMPL)
