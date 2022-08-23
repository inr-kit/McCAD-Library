// C++
#include <array>
// McCAD
#include "extendedFaceCreator.hpp"
// OCC
#include <gp_Pnt.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

McCAD::Tools::ExtendedFaceCreator::ExtendedFaceCreator(){
}

McCAD::Tools::ExtendedFaceCreator::ExtendedFaceCreator(const double& edgeTolerance)
    : edgeTolerance{edgeTolerance}{}

McCAD::Tools::ExtendedFaceCreator::~ExtendedFaceCreator(){}

TopoDS_Face
McCAD::Tools::ExtendedFaceCreator::generateExtendedPlane(
        const TopoDS_Face& face, const double& boxDiagonalLength){
    Handle_Geom_Surface surface = BRep_Tool::Surface(face);
    std::array<double, 4> uvParameters;
    BRepTools::UVBounds(face, uvParameters[0], uvParameters[1], uvParameters[2],
                        uvParameters[3]);
    std::array<double, 4> newUVParameters;
    newUVParameters[0] = uvParameters[0] - boxDiagonalLength;
    newUVParameters[1] = uvParameters[1] + boxDiagonalLength;
    newUVParameters[2] = uvParameters[2] - boxDiagonalLength;
    newUVParameters[3] = uvParameters[3] + boxDiagonalLength;
    TopoDS_Face newFace = BRepBuilderAPI_MakeFace(surface, newUVParameters[0],
            newUVParameters[1], newUVParameters[2], newUVParameters[3],
            edgeTolerance);
    return newFace;
}

TopoDS_Face
McCAD::Tools::ExtendedFaceCreator::generateExtendedCone(
    const TopoDS_Face& face, const double& boxDiagonalLength) {
    TopoDS_Face face = boundSurfaceCyl->accessSImpl()->face;
    Handle_Geom_Surface surface = BRep_Tool::Surface(face);
    std::array<Standard_Real, 4> uvParameters;
    // UV parameters in class grom_Cylindrical_Surface: U1 = 0 and U2 = 2*PI.
    BRepTools::UVBounds(face, uvParameters[0], uvParameters[1], uvParameters[2],
        uvParameters[3]);

    rangeRadian = std::abs(uvParameters[1] - uvParameters[0]);
    std::array<Standard_Real, 4> newUVParameters;
    newUVParameters[0] = 0;
    newUVParameters[1] = 2 * M_PI;
    newUVParameters[2] = uvParameters[2] - boxDiagonalLength;
    newUVParameters[3] = uvParameters[3] + boxDiagonalLength;

    boundSurfaceCyl->accessSImpl()->extendedFace =
        BRepBuilderAPI_MakeFace(surface, newUVParameters[0], newUVParameters[1],
            newUVParameters[2], newUVParameters[3], tolerance);
}
