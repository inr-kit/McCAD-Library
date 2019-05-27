#include "decomposition.hpp"

McCAD::Decomp::Decomposition::Decomposition(
    const McCAD::Interface::SolidsCollection &raw) 
    : raw{raw} {
    }

void McCAD::Decomp::Decomposition::run() {
  std::cout << "Testing linking of libraries using cmake" << std::endl;
}

