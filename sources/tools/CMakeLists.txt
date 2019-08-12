add_library(Tools
    tools.cpp
    TypeName.cpp
    PlaneComparator.cpp)
install (TARGETS Tools DESTINATION ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(Tools OCC)

add_library(Tools_impl tools_impl.cpp)
install (TARGETS Tools_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(Tools_impl OCC)

