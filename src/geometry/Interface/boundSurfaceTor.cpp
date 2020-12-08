// McCAD
#include "boundSurfaceTor.hpp"
#include "boundSurfaceTor_impl.hpp"

McCAD::Geometry::BoundSurfaceTor::BoundSurfaceTor()
  : pBSTImpl{std::make_unique<Impl>(this)}, BoundSurface::BoundSurface() {
}

McCAD::Geometry::BoundSurfaceTor::~BoundSurfaceTor(){
}

McCAD::Geometry::BoundSurfaceTor::Impl*
McCAD::Geometry::BoundSurfaceTor::accessBSTImpl() const{
  return pBSTImpl.get();
}
