# McCAD library
message("")
file(GLOB SRC_FILES "*.cpp")
set(LINK_LIBS_INT)
set(LINK_LIBS_DEP)
include_directories(${PROJECT_SOURCE_DIR}/src/McCAD
                    ${PROJECT_BINARY_DIR}/src/McCAD)
McCAD_install_exe(McCAD)
