// McCAD
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::Decompose::~Decompose(){
}

const McCAD::Decomposition::Decompose::Impl*
McCAD::Decomposition::Decompose::accessImpl() const{
  return pImpl.get();
}

void McCAD::Decomposition::Decompose::perform(){
  // Get the input solids list from the loaded STEP file.
  pImpl->getInputSolidsList();
  // Split the solids in the list if it is a compound solid.
  pImpl->splitInputSolids();
}
