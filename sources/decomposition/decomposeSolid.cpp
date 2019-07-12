// McCAD
#include "decomposeSolid.hpp"
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::DecomposeSolid()
  : pDecomposeSolidImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::DecomposeSolid::~DecomposeSolid(){
}

McCAD::Decomposition::DecomposeSolid::Impl*
McCAD::Decomposition::DecomposeSolid::accessImpl() const{
  return pDecomposeSolidImpl.get();
}
