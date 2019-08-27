// McCAD
#include "surface_impl.hpp"

void
McCAD::Decomposition::Surface::Impl::initiate(const TopoDS_Face& aFace){
  face = aFace;
}

