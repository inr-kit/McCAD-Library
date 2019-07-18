// McCAD
#include "boundSurface.hpp"
#include "boundSurface_impl.hpp"

McCAD::Decomposition::BoundSurface::BoundSurface()
  : pBSImpl{std::make_unique<Impl>(this)}, Surface::Surface() {
}

McCAD::Decomposition::BoundSurface::~BoundSurface(){
}

McCAD::Decomposition::BoundSurface::Impl*
McCAD::Decomposition::BoundSurface::accessBSImpl() const{
  return pBSImpl.get();
}
