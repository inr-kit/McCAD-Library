// McCAD
#include "surface_impl.hpp"

void
McCAD::Decomposition::BoundSurfacePlane::Impl::initiate(const TopoDS_Face& aFace){
  face = aFace;
}

