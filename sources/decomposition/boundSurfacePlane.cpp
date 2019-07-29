// McCAD
#include "boundSurfacePlane.hpp"
#include "boundSurfacePlane_impl.hpp"

McCAD::Decomposition::BoundSurfacePlane::BoundSurfacePlane()
  : pBSPImpl{std::make_unique<Impl>(this)}, BoundSurface::BoundSurface() {
}

McCAD::Decomposition::BoundSurfacePlane::~BoundSurfacePlane(){
}

McCAD::Decomposition::BoundSurfacePlane::Impl*
McCAD::Decomposition::BoundSurfacePlane::accessBSPImpl() const{
  return pBSPImpl.get();
}
