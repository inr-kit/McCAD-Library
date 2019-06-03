add_library(General general.cpp)
install (TARGETS General DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES general.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)

add_library(GeneralDetail generaldetail.cpp)
install (TARGETS GeneralDetail DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES generaldetail.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
