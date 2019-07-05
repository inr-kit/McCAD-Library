// McCAD
#include "decomposeSolid.hpp"
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::DecomposeSolid()
  : pImpl{std::make_unique<Impl>(this)}{
}

McCAD::Decomposition::DecomposeSolid::~DecomposeSolid(){
}

McCAD::Decomposition::DecomposeSolid::Impl*
McCAD::Decomposition::DecomposeSolid::accessImpl() const{
  return pImpl.get();
}
