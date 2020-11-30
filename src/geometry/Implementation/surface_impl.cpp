// McCAD
#include "surface_impl.hpp"

void
McCAD::Geometry::Surface::Impl::initiate(const TopoDS_Face& aFace){
  face = aFace;
}

