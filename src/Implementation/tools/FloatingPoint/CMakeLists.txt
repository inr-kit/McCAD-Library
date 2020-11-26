add_library(FloatingPoint INTERFACE)

install(FILES
  StaticPow.hpp
  BinaryDecomposition.hpp
  BinaryDecomposition.tpp
  DecimalDecomposition.hpp
  DecimalDecomposition.tpp
  Comparison.hpp
  Comparison.tpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/tools)

