// McCAD
#include "surface.hpp"
#include "surface_impl.hpp"

McCAD::Decomposition::Surface::Surface()
  : pSImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::Surface::~Surface(){
}

McCAD::Decomposition::Surface::Impl*
McCAD::Decomposition::Surface::accessSImpl() const{
  return pSImpl.get();
}
