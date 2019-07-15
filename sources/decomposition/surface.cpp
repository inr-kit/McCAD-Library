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

void
McCAD::Decomposition::Surface::setSurfaceType(const std::string& surfaceType){
  pSImpl->surfaceType = surfaceType;
}

std::string
McCAD::Decomposition::Surface::getSurfaceType(){
  return pSImpl->surfaceType;
}
