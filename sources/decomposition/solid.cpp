// McCAD
#include "solid.hpp"
#include "solid_impl.hpp"

McCAD::Decomposition::Solid::Solid()
  : pSImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::Solid::~Solid(){
}

McCAD::Decomposition::Solid::Impl*
McCAD::Decomposition::Solid::accessSImpl() const{
  return pSImpl.get();
}
