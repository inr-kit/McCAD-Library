// McCAD
#include "boundSurface_impl.hpp"

McCAD::Decomposition::BoundSurface::Impl::Impl(McCAD::Decomposition::BoundSurface* bndSurface) : bndSurface{bndSurface}{
}

void
McCAD::Decomposition::BoundSurface::Impl::initiate(){
}

Standard_Boolean
generateMesh(const Standard_Real& meshDeflection){
  return Standard_True;
}
