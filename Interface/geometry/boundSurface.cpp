// McCAD
#include "boundSurface.hpp"
#include "boundSurface_impl.hpp"

McCAD::Geometry::BoundSurface::BoundSurface()
  : pBSImpl{std::make_unique<Impl>(this)}, Surface::Surface() {
}

McCAD::Geometry::BoundSurface::~BoundSurface(){
}

McCAD::Geometry::BoundSurface::Impl*
McCAD::Geometry::BoundSurface::accessBSImpl() const{
  return pBSImpl.get();
}

Standard_Boolean
McCAD::Geometry::BoundSurface::operator == (const McCAD::Geometry::BoundSurface& that){
  return pBSImpl->isEqual(that);
}

Standard_Boolean
McCAD::Geometry::BoundSurface::operator << (const McCAD::Geometry::BoundSurface& that){
  return pBSImpl->canFuse(that);
}

