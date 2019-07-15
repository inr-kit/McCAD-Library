// McCAD
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose(const McCAD::General::InputData& inputData)
  : pDImpl{std::make_unique<Impl>(inputData)}{
}

McCAD::Decomposition::Decompose::~Decompose(){
}

McCAD::Decomposition::Decompose::Impl*
McCAD::Decomposition::Decompose::accessDImpl() const{
  return pDImpl.get();
}
