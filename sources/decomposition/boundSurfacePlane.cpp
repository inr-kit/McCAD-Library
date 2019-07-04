// McCAD
#include "boundSurfacePlane.hpp"
#include "boundSurfacePlane_impl.hpp"

McCAD::Decomposition::BoundSurfacePlane::BoundSurfacePlane()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::BoundSurfacePlane::~BoundSurfacePlane(){
}

McCAD::Decomposition::BoundSurfacePlane::Impl*
McCAD::Decomposition::BoundSurfacePlane::accessImpl() const{
  return pImpl.get();
}
