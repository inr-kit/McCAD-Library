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
  TopoDS_Face face = boundSurfacePlane->accessSImpl()->face; 
  Handle_Geom_Surface surface = BRep_Tool::Surface(face);
  //std::cout << "calculating parameters" << std::endl;
  std::array<Standard_Real, 4> uvParameters;
  BRepTools::UVBounds(face, uvParameters[0], uvParameters[1], uvParameters[2],
		      uvParameters[3]);
  //std::cout << "U: " << uMid << ", V: " << vMid << std::endl;
  std::array<Standard_Real, 4> newUVParameters;
  newUVParameters[0] = uvParameters[0] - boxSquareLength;
  newUVParameters[1] = uvParameters[1] + boxSquareLength;
  newUVParameters[2] = uvParameters[2] - boxSquareLength;
  newUVParameters[3] = uvParameters[3] + boxSquareLength;

  boundSurfacePlane->accessSImpl()->extendedFace =
    BRepBuilderAPI_MakeFace(surface, newUVParameters[0], newUVParameters[1],
			    newUVParameters[2], newUVParameters[3],
			    degenerateEdgesTolerance);
}
