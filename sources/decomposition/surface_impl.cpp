// McCAD
#include "surface_impl.hpp"

void
McCAD::Decomposition::Surface::Impl::initiate(const TopoDS_Face& aFace){
  face = aFace;
}

Standard_Boolean
McCAD::Decomposition::Surface::Impl::isEqual(const std::unique_ptr<McCAD::Decomposition::Surface::Impl>& that){
  return preroc.accessImpl()->isSameSurface(face, that->face);
}

