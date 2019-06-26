// McCAD
#include "decomposeSolid.hpp"
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::DecomposeSolid(const TopoDS_Solid& solid)
  : pImpl{std::make_unique<Impl>(solid)}{
}

McCAD::Decomposition::DecomposeSolid::~DecomposeSolid(){
}

McCAD::Decomposition::DecomposeSolid::Impl*
McCAD::Decomposition::DecomposeSolid::accessImpl() const{
  return pImpl.get();
}
