add_library(CONVERSION_IMPL OBJECT
    #BVHCreator.cpp
    voidGenerator.cpp
    conversion_impl.cpp)

install(FILES
    #BVHCreator.hpp
    voidGenerator.hpp
    conversion_impl.hpp
    DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/conversion)

target_link_libraries(CONVERSION_IMPL
    #MOAB
    OCC
    TOOLS_IFACE)
