// McCAD
#include "boundSurfaceCyl.hpp"
#include "boundSurfaceCyl_impl.hpp"

McCAD::Geometry::BoundSurfaceCyl::BoundSurfaceCyl()
  : pBSCImpl{std::make_unique<Impl>(this)}, BoundSurface::BoundSurface() {
}

McCAD::Geometry::BoundSurfaceCyl::~BoundSurfaceCyl(){
}

McCAD::Geometry::BoundSurfaceCyl::Impl*
McCAD::Geometry::BoundSurfaceCyl::accessBSCImpl() const{
  return pBSCImpl.get();
}
