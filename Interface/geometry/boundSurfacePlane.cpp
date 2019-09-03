// McCAD
#include "boundSurfacePlane.hpp"
#include "boundSurfacePlane_impl.hpp"

McCAD::Geometry::BoundSurfacePlane::BoundSurfacePlane()
  : pBSPImpl{std::make_unique<Impl>(this)}, BoundSurface::BoundSurface() {
}

McCAD::Geometry::BoundSurfacePlane::~BoundSurfacePlane(){
}

McCAD::Geometry::BoundSurfacePlane::Impl*
McCAD::Geometry::BoundSurfacePlane::accessBSPImpl() const{
  return pBSPImpl.get();
}
