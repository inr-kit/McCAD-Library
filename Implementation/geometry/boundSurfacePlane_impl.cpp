// McCAD
//McCAD
#include "boundSurfacePlane_impl.hpp"

McCAD::Geometry::BoundSurfacePlane::Impl::Impl(
        McCAD::Geometry::BoundSurfacePlane* backReference)
    : boundSurfacePlane{backReference}{
}

McCAD::Geometry::BoundSurfacePlane::Impl::~Impl(){
}

void
McCAD::Geometry::BoundSurfacePlane::Impl::generateExtendedPlane(
        const Standard_Real& boxDiagonalLength,
        Standard_Real degenerateEdgesTolerance){
    TopoDS_Face face = boundSurfacePlane->accessSImpl()->face;
    Handle_Geom_Surface surface = BRep_Tool::Surface(face);
    std::array<Standard_Real, 4> uvParameters;
    BRepTools::UVBounds(face, uvParameters[0], uvParameters[1], uvParameters[2],
                        uvParameters[3]);
    std::array<Standard_Real, 4> newUVParameters;
    newUVParameters[0] = uvParameters[0] - boxDiagonalLength;
    newUVParameters[1] = uvParameters[1] + boxDiagonalLength;
    newUVParameters[2] = uvParameters[2] - boxDiagonalLength;
    newUVParameters[3] = uvParameters[3] + boxDiagonalLength;
    /* //debug
    std::cout << "old UV: " << uvParameters[0] << " ," << uvParameters[1] <<
                 " ," << uvParameters[2] << " ," << uvParameters[3] << std::endl;
    std::cout << "new UV: " << newUVParameters[0] << " ," << newUVParameters[1] <<
                 " ," << newUVParameters[2] << " ," << newUVParameters[3] << std::endl;
    */ //debug
    boundSurfacePlane->accessSImpl()->extendedFace =
    BRepBuilderAPI_MakeFace(surface, newUVParameters[0], newUVParameters[1],
          newUVParameters[2], newUVParameters[3], degenerateEdgesTolerance);
}
