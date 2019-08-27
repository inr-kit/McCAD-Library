add_library(FloatingPoint INTERFACE)

target_sources(FloatingPoint INTERFACE
  StaticPow.hpp
  BinaryDecomposition.hpp
  BinaryDecomposition.tpp
  DecimalDecomposition.hpp
  DecimalDecomposition.tpp
  Comparison.hpp
  Comparison.tpp)

target_include_directories(FloatingPoint INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR})
