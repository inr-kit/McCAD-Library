// McCAD
#include "surface.hpp"
#include "surface_impl.hpp"

McCAD::Geometry::Surface::Surface()
  : pSImpl{std::make_unique<Impl>()}{
}

McCAD::Geometry::Surface::~Surface(){
}

McCAD::Geometry::Surface::Impl*
McCAD::Geometry::Surface::accessSImpl() const{
  return pSImpl.get();
}

void
McCAD::Geometry::Surface::setSurfaceType(const std::string& surfaceType){
  pSImpl->surfaceType = surfaceType;
}

std::string
McCAD::Geometry::Surface::getSurfaceType(){
  return pSImpl->surfaceType;
}
