# McCAD library
message("")
file(GLOB SRC_FILES "*.cpp")
#set(LINK_LIBS_INT INPUTDATA_IFACE IO_IFACE TOOLS_IFACE GEOMETRY_IFACE
#    DECOMPOSITION_IFACE CONVERSION_IFACE)
set(LINK_LIBS_INT TOOLS_IFACE)
set(LINK_LIBS_DEP "")
include_directories(${PROJECT_SOURCE_DIR}/src/McCAD
    ${PROJECT_BINARY_DIR}/src/McCAD
    #${PROJECT_SOURCE_DIR}/src/io/Interface
    ${PROJECT_SOURCE_DIR}/src/general/Interface
    ${PROJECT_SOURCE_DIR}/src/geometry/Interface
    ${PROJECT_SOURCE_DIR}/src/tools/Interface
    #${PROJECT_SOURCE_DIR}/src/decomposition/Interface
    #${PROJECT_SOURCE_DIR}/src/conversion/Interface)
    )
McCAD_install_exe(McCAD)
