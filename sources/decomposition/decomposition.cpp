// McCAD
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose(const McCAD::General::InputData& inputData)
  : pDecomposeImpl{std::make_unique<Impl>(inputData)}{
}

McCAD::Decomposition::Decompose::~Decompose(){
}

McCAD::Decomposition::Decompose::Impl*
McCAD::Decomposition::Decompose::accessImpl() const{
  return pDecomposeImpl.get();
}
