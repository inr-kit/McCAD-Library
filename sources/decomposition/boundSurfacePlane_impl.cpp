// McCAD
#include "boundSurfacePlane_impl.hpp"

McCAD::Decomposition::BoundSurfacePlane::Impl(McCAD::Decomposition::BoundSurfacePlane* backReference) : boundSurfacePlane{backReference}{
}

void
McCAD::Decomposition::BoundSurfacePlane::Impl::generateExtPlane(const Standard_Real& boxSquareLength){
  TopLoc_Location location;
  Handle_Geometry_Surface surrface = BRep_Tool::Surface(boundSurfacePlane->accessSImpl()->face, location);
  std::vector<Standard_Real> uvParameters;
  BRepTools::UVBounds(boundSurfacePlane->accessSImpl()->face, uvParameters[0], uvParameters[1], uvParameters[2], uvParameters[3]);
  
  
}
