// McCAD
#include "decomposeSolid.hpp"
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::DecomposeSolid()
  : pDSImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::DecomposeSolid::~DecomposeSolid(){
}

McCAD::Decomposition::DecomposeSolid::Impl*
McCAD::Decomposition::DecomposeSolid::accessDSImpl() const{
  return pDSImpl.get();
}
