add_library(Materials materials.cpp)
install (TARGETS Materials DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES materials.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
