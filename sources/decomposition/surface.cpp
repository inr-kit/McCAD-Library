// McCAD
#include "surface.hpp"
#include "surface_impl.hpp"

McCAD::Decomposition::Surface::Surface()
  : pImpl{std::make_unique<Impl>(this)}{
}

McCAD::Decomposition::Surface::~Surface(){
}

McCAD::Decomposition::Surface::Impl*
McCAD::Decomposition::Surface::accessImpl() const{
  return pImpl.get();
}

