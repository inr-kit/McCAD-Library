// McCAD
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::Decompose::~Decompose(){
}

McCAD::Decomposition::Decompose::Impl*
McCAD::Decomposition::Decompose::accessImpl(){
  return pImpl.get();
}

void McCAD::Decomposition::Decompose::perform(McCAD::General::InputData& inputData){
  pImpl->decompose(inputData.accessImpl()->inputSolidsList);
}
