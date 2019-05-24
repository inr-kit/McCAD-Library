#include "decomposition.hpp"

McCAD::Decomp::Decomposition::Decomposition(
    const McCAD::Universal::SolidsCollection &raw) 
    : raw{raw} {
    }

void McCAD::Decomp::Decomposition::run() {
  std::cout << "Testing linking of libraries using cmake" << std::endl;
}

