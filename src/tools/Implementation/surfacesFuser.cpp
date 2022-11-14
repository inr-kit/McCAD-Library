// McCAD
#include "surfacesFuser.hpp"

McCAD::Tools::SurfacesFuser::SurfacesFuser(double precision,
                                           double edgeTolerance)
    : precision{precision}, edgeTolerance{edgeTolerance}{
}

/** ********************************************************************
* @brief    An operator that calls the specified fuser for the given face types.
* @param    firstFace is a OCCT face.
* @param    secondFace is a OCCT face.
* @return   A new OCCT face.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
std::optional<TopoDS_Face>
McCAD::Tools::SurfacesFuser::operator()(const TopoDS_Face& firstFace,
                                        const TopoDS_Face& secondFace){
    GeomAdaptor_Surface firstAdaptor{BRep_Tool::Surface(firstFace)};
    if (firstAdaptor.GetType() == GeomAbs_Plane)
        return fusePlanes(firstFace, secondFace);
    else if (firstAdaptor.GetType() == GeomAbs_Cylinder)
        return fuseCyls(firstFace, secondFace);
    else if (firstAdaptor.GetType() == GeomAbs_Cone)
        return fuseCones(firstFace, secondFace);
    return std::nullopt;
}

/** ********************************************************************
* @brief    A function that calculates UV bounds of a surface.
* @param    face is a OCCT face.
* @return   An array of UV bounds.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
std::array<double, 4>
McCAD::Tools::SurfacesFuser::uvBounds(const TopoDS_Face& face) const{
    std::array<double, 4> uvBounds;
    BRepTools::UVBounds(face, uvBounds[0], uvBounds[1], uvBounds[2], uvBounds[3]);
    for(auto& uv : uvBounds)
        if(uv < precision) uv = 0.0;
    return uvBounds;
}

/** ********************************************************************
* @brief    A function that fuses two planar faces and creates a new OCCT face.
* @param    first is a OCCT face.
* @param    second is a OCCT face.
* @return   A new OCCT face.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
TopoDS_Face
McCAD::Tools::SurfacesFuser::fusePlanes(const TopoDS_Face& first,
                                        const TopoDS_Face& second){
    auto firstUV = uvBounds(first);
    auto secondUV = uvBounds(second);
    std::array<double, 4> newUV;
    newUV[0] = (firstUV[0] <= secondUV[0]) ? firstUV[0] : secondUV[0];
    newUV[1] = (firstUV[1] >= secondUV[1]) ? firstUV[1] : secondUV[1];
    newUV[2] = (firstUV[2] <= secondUV[2]) ? firstUV[2] : secondUV[2];
    newUV[3] = (firstUV[3] >= secondUV[3]) ? firstUV[3] : secondUV[3];
    Handle_Geom_Surface newSurface = BRep_Tool::Surface(first);
    TopoDS_Face newFace = BRepBuilderAPI_MakeFace(newSurface, newUV[0], newUV[1],
            newUV[2], newUV[3], edgeTolerance).Face();
    newFace.Orientation(first.Orientation());
    return newFace;
}

/** ********************************************************************
* @brief    A function that fuses two cylindrical faces and creates a new OCCT face.
* @param    first is a OCCT face.
* @param    second is a OCCT face.
* @return   A new OCCT face.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
TopoDS_Face
McCAD::Tools::SurfacesFuser::fuseCyls(const TopoDS_Face& first,
                                      const TopoDS_Face& second){
    auto firstUV = uvBounds(first);
    auto secondUV = uvBounds(second);
    std::array<double, 4> newUV;
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
            newUV[2], newUV[3], edgeTolerance).Face();
    newFace.Orientation(first.Orientation());
    return newFace;
}

/** ********************************************************************
* @brief    A function that fuses two conical faces and creates a new OCCT face.
* @param    first is a OCCT face.
* @param    second is a OCCT face.
* @return   A new OCCT face.
* @date     25/08/2022
* @modified 
* @author   Moataz Harb
* **********************************************************************/
TopoDS_Face
McCAD::Tools::SurfacesFuser::fuseCones(const TopoDS_Face& first,
                                       const TopoDS_Face& second) {
    auto firstUV = uvBounds(first);
    auto secondUV = uvBounds(second);
    std::array<double, 4> newUV;
    if (std::abs(firstUV[1] - firstUV[0]) + std::abs(secondUV[1] - secondUV[0]) >=
        2 * M_PI) {
        //Two surfaces form a closed cone.
        newUV[0] = 0.0;
        newUV[1] = 2 * M_PI;
    }
    else {
        newUV[0] = (firstUV[0] <= secondUV[0]) ? firstUV[0] : secondUV[0];
        newUV[1] = (firstUV[1] >= secondUV[1]) ? firstUV[1] : secondUV[1];
    }
    newUV[2] = (firstUV[2] <= secondUV[2]) ? firstUV[2] : secondUV[2];
    newUV[3] = (firstUV[3] >= secondUV[3]) ? firstUV[3] : secondUV[3];
    Handle_Geom_Surface newSurface = BRep_Tool::Surface(first);
    TopoDS_Face newFace = BRepBuilderAPI_MakeFace(newSurface, newUV[0], newUV[1],
        newUV[2], newUV[3], edgeTolerance).Face();
    newFace.Orientation(first.Orientation());
    return newFace;
}
