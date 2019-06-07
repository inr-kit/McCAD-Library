// McCAD
#include "inputdata.hpp"
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::Decompose::~Decompose(){
}

void McCAD::Decomposition::Decompose::perform(){
  // Populate the input solids list.
  pImpl->getInputSolidsList();
}
