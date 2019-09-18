// McCAD
#include "boundSurfacePlane_impl.hpp"

McCAD::Geometry::BoundSurfacePlane::Impl::Impl(McCAD::Geometry::BoundSurfacePlane* backReference) : boundSurfacePlane{backReference}{
}

McCAD::Geometry::BoundSurfacePlane::Impl::~Impl(){
}

void
McCAD::Geometry::BoundSurfacePlane::Impl::generateExtendedPlane(const Standard_Real& boxSquareLength,
								Standard_Real degenerateEdgesTolerance){
  //std::cout << "boxSquareLength, " << boxSquareLength << std::endl;
  Handle_Geom_Surface surface = BRep_Tool::Surface(boundSurfacePlane->accessSImpl()->face);
  //std::cout << "calculating parameters" << std::endl;
  std::array<Standard_Real, 4> uvParameters;
  BRepTools::UVBounds(boundSurfacePlane->accessSImpl()->face, uvParameters[0],
		      uvParameters[1], uvParameters[2], uvParameters[3]);

  // Calculate new parameters.
  Standard_Real uMid = (uvParameters[0] + uvParameters[1])/2.0;
  Standard_Real vMid = (uvParameters[2] + uvParameters[3])/2.0;
  //std::cout << "U: " << uMid << ", V: " << vMid << std::endl;

  std::array<Standard_Real, 4> newUVParameters;
  newUVParameters[0] = uMid - boxSquareLength/0.5;
  newUVParameters[1] = uMid + boxSquareLength/0.5;
  newUVParameters[2] = vMid - boxSquareLength/0.5;
  newUVParameters[3] = vMid + boxSquareLength/0.5;

  boundSurfacePlane->accessSImpl()->extendedFace =
    BRepBuilderAPI_MakeFace(surface, newUVParameters[0], newUVParameters[1],
			    newUVParameters[2], newUVParameters[3],
			    degenerateEdgesTolerance);
}
