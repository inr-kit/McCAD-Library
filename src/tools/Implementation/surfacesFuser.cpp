// McCAD
#include "surfacesFuser.hpp"
// OCC
#include <Message_ProgressRange.hxx>

McCAD::Tools::SurfacesFuser::SurfacesFuser(Standard_Real zeroTolerance,
                                           Standard_Real tolerance)
    : zeroTolerance{zeroTolerance}, tolerance{tolerance}{
}

std::optional<TopoDS_Face>
McCAD::Tools::SurfacesFuser::operator()(const TopoDS_Face& firstFace,
                                        const TopoDS_Face& secondFace){
    GeomAdaptor_Surface firstAdaptor{BRep_Tool::Surface(firstFace)};
    if (firstAdaptor.GetType() == GeomAbs_Plane)
        return fusePlanes(firstFace, secondFace);
    else if (firstAdaptor.GetType() == GeomAbs_Cylinder)
        return fuseCyls(firstFace, secondFace);
}

std::array<Standard_Real, 4>
McCAD::Tools::SurfacesFuser::uvBounds(const TopoDS_Face& face) const{
    std::array<Standard_Real, 4> uvBounds;
    BRepTools::UVBounds(face, uvBounds[0], uvBounds[1], uvBounds[2], uvBounds[3]);
    for(auto& uv : uvBounds)
        if(uv <= zeroTolerance) uv = 0.0;
    return uvBounds;
}

TopoDS_Face
McCAD::Tools::SurfacesFuser::fusePlanes(const TopoDS_Face& first,
                                        const TopoDS_Face& second){
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

TopoDS_Face
McCAD::Tools::SurfacesFuser::fuseCyls(const TopoDS_Face& first,
                                      const TopoDS_Face& second){
    auto firstUV = uvBounds(first);
    auto secondUV = uvBounds(second);
    std::array<Standard_Real, 4> newUV;
    if (std::abs(firstUV[1] - firstUV[0]) + std::abs(secondUV[1] - secondUV[0]) >=
            2*M_PI){
        //Two surfaces form a closed cylinder.
        newUV[0] = 0.0;
        newUV[1] = 2*M_PI;
    } else {
        newUV[0] = (firstUV[0] <= secondUV[0]) ? firstUV[0] : secondUV[0];
        newUV[1] = (firstUV[1] >= secondUV[1]) ? firstUV[1] : secondUV[1];
    }
    newUV[2] = (firstUV[2] <= secondUV[2]) ? firstUV[2] : secondUV[2];
    newUV[3] = (firstUV[3] >= secondUV[3]) ? firstUV[3] : secondUV[3];
    Handle_Geom_Surface newSurface = BRep_Tool::Surface(first);
    TopoDS_Face newFace = BRepBuilderAPI_MakeFace(newSurface, newUV[0], newUV[1],
            newUV[2], newUV[3], tolerance).Face();
    newFace.Orientation(first.Orientation());
    return newFace;
}
