// McCAD
#include "decomposeSolid.hpp"
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::DecomposeSolid(const IO::InputConfig& inputConfig)
  : pDSImpl{std::make_unique<Impl>(inputConfig)}{
}

McCAD::Decomposition::DecomposeSolid::~DecomposeSolid(){
}

McCAD::Decomposition::DecomposeSolid::Impl*
McCAD::Decomposition::DecomposeSolid::accessDSImpl() const{
  return pDSImpl.get();
}
