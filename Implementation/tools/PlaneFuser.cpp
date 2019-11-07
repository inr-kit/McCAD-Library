// McCAD
#include "PlaneFuser.hpp"

McCAD::Tools::PlaneFuser::PlaneFuser(Standard_Real zeroTolerance,
                                     Standard_Real tolerance)
    : zeroTolerance{zeroTolerance} , tolerance{tolerance}{
}

TopoDS_Face
McCAD::Tools::PlaneFuser::operator()(const TopoDS_Face& first,
                                     const TopoDS_Face& second) const{
    auto firstUV = uvBounds(first);
    auto secondUV = uvBounds(second);
    std::array<Standard_Real, 4> newUV;
    newUV[0] = (firstUV[0] <= secondUV[0]) ? firstUV[0] : secondUV[0];
    newUV[1] = (firstUV[1] >= secondUV[1]) ? firstUV[1] : secondUV[1];
    newUV[2] = (firstUV[2] <= secondUV[2]) ? firstUV[2] : secondUV[2];
    newUV[3] = (firstUV[3] >= secondUV[3]) ? firstUV[3] : secondUV[3];
    Handle_Geom_Surface newSurface = BRep_Tool::Surface(first);
    TopoDS_Face newFace = BRepBuilderAPI_MakeFace(newSurface, newUV[0], newUV[1],
            newUV[2], newUV[3], tolerance).Face();
    newFace.Orientation(first.Orientation());
    return newFace;
}

std::array<Standard_Real, 4>
McCAD::Tools::PlaneFuser::uvBounds(const TopoDS_Face& face) const{
    std::array<Standard_Real, 4> uvBounds;
    BRepTools::UVBounds(face, uvBounds[0], uvBounds[1], uvBounds[2], uvBounds[3]);
    for(auto& uv : uvBounds)
        if(uv <= zeroTolerance) uv = 0.0;
    return uvBounds;
}
