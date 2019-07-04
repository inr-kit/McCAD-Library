// McCAD
#include "surface.hpp"
#include "surface_impl.hpp"

McCAD::Decomposition::Surface::Surface()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::Surface::~Surface(){
}

McCAD::Decomposition::Surface::Impl*
McCAD::Decomposition::Surface::accessImpl() const{
  return pImpl.get();
}

void
McCAD::Decomposition::Surface::setSurfaceType(const std::string& surfacetype){
  pImpl->surfaceType = surfacetype;
}

std::string
McCAD::Decomposition::Surface::getSurfaceType(){
  return pImpl->surfaceType;
}

void
McCAD::Decomposition::Surface::setSurfaceNumber(const Standard_Integer& surfaceNumber){
  pImpl->surfaceNumber = surfaceNumber;
}

Standard_Integer
McCAD::Decomposition::Surface::getSurfaceNUmber(){
  return pImpl->surfaceNumber;
}
