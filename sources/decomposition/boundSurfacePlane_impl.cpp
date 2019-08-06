// McCAD
#include "boundSurfacePlane_impl.hpp"

McCAD::Decomposition::BoundSurfacePlane::Impl::Impl(McCAD::Decomposition::BoundSurfacePlane* backReference) : boundSurfacePlane{backReference}{
}

McCAD::Decomposition::BoundSurfacePlane::Impl::~Impl(){
}

void
McCAD::Decomposition::BoundSurfacePlane::Impl::generateExtendedPlane(const Standard_Real& boxSquareLength, Standard_Real tolerance){
  //std::cout << "boxSquareLength, " << boxSquareLength << std::endl;
  TopLoc_Location location;
  Handle_Geom_Surface surface = BRep_Tool::Surface(boundSurfacePlane->accessSImpl()->face, location);
  //std::cout << "calculating parameters" << std::endl;
  std::vector<Standard_Real> uvParameters(4);
  BRepTools::UVBounds(boundSurfacePlane->accessSImpl()->face, uvParameters[0], uvParameters[1], uvParameters[2], uvParameters[3]);

  // Calculate new parameters.
  Standard_Real uMid = (uvParameters[0] + uvParameters[1])/2.0;
  Standard_Real vMid = (uvParameters[2] + uvParameters[3])/2.0;
  //std::cout << "U: " << uMid << ", V: " << vMid << std::endl;

  std::vector<Standard_Real> newUVParameters(4);
  newUVParameters[0] = uMid - boxSquareLength/1.0;
  newUVParameters[1] = uMid + boxSquareLength/1.0;
  newUVParameters[2] = vMid - boxSquareLength/1.0;
  newUVParameters[3] = vMid + boxSquareLength/1.0;

  boundSurfacePlane->accessSImpl()->extendedFace = BRepBuilderAPI_MakeFace(surface, newUVParameters[0], newUVParameters[1], newUVParameters[2], newUVParameters[3], tolerance);
}
