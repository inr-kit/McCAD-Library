add_library(SolidsCollection solidscollection.cpp)
install (TARGETS SolidsCollection DESTINATION ${PROJECT_BINARY_DIR}/bin)
install (FILES solidscollection.hpp DESTINATION ${PROJECT_BINARY_DIR}/include)
