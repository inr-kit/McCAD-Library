// McCAD
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose(const McCAD::General::InputData& inputData)
  : pImpl{std::make_unique<Impl>(inputData)}{
}

McCAD::Decomposition::Decompose::~Decompose(){
}

McCAD::Decomposition::Decompose::Impl*
McCAD::Decomposition::Decompose::accessImpl() const{
  return pImpl.get();
}
