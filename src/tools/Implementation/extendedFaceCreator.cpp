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
#include <BRepAdaptor_Surface.hxx>
#include <gp_Cone.hxx>
#include <ElSLib.hxx>

McCAD::Tools::ExtendedFaceCreator::ExtendedFaceCreator(){
}

McCAD::Tools::ExtendedFaceCreator::ExtendedFaceCreator(const double& edgeTolerance)
    : edgeTolerance{edgeTolerance}{}

McCAD::Tools::ExtendedFaceCreator::~ExtendedFaceCreator(){}

TopoDS_Face
McCAD::Tools::ExtendedFaceCreator::generateExtendedPlane(
        const TopoDS_Face& face, const double& boxDiagonalLength){
    // To fix the issue of some extended surfaces not extending beyound the solid
    // BB boundaries, the ectension multiplier is introduced.
    int extensionMultiplier{ 10 };
    Handle_Geom_Surface surface = BRep_Tool::Surface(face);
    std::array<double, 4> uvParameters, newUVParameters;
    BRepTools::UVBounds(face, uvParameters[0], uvParameters[1], uvParameters[2],
                        uvParameters[3]);
    newUVParameters[0] = uvParameters[0] - extensionMultiplier * boxDiagonalLength;
    newUVParameters[1] = uvParameters[1] + extensionMultiplier * boxDiagonalLength;
    newUVParameters[2] = uvParameters[2] - extensionMultiplier * boxDiagonalLength;
    newUVParameters[3] = uvParameters[3] + extensionMultiplier * boxDiagonalLength;
    TopoDS_Face newFace = BRepBuilderAPI_MakeFace(surface, newUVParameters[0],
            newUVParameters[1], newUVParameters[2], newUVParameters[3],
            edgeTolerance);
    return newFace;
}

/** ********************************************************************
* @brief    A function that creates an extended conical face.
* @param    face is a OCCT face.
* @param    boxDiagonalLength is the calculated diagonal length of the solid OBB.
* @return   a OCCT face.
* @date     31/12/2020
* @modified 23/08/2022
* @author   Moataz Harb
* **********************************************************************/
TopoDS_Face
McCAD::Tools::ExtendedFaceCreator::generateExtendedCone(
    const TopoDS_Face& face, const double& boxDiagonalLength) {
    // Get surface parameters.
    std::array<double, 4> uvParameters;
    // UV parameters in class geom_Conical_Surface: U1 = 0 and U2 = 2*PI.
    BRepTools::UVBounds(face, uvParameters[0], uvParameters[1], uvParameters[2],
        uvParameters[3]);
    // Get apex and the U V parameters of it.
    BRepAdaptor_Surface surface{ face, true };
    gp_Cone cone = surface.Cone();
    gp_Pnt apex = cone.Apex();
    double UApex, VApex;
    ElSLib::Parameters(cone, apex, UApex, VApex);
    std::array<double, 4> newUVParameters;
    newUVParameters[0] = 0;
    newUVParameters[1] = 2 * M_PI;
    if (VApex < newUVParameters[2]) {
        // VMin of the cone is higher than that of the Apex. Extend the cone to the Apex.
        newUVParameters[2] = VApex;
        newUVParameters[3] = uvParameters[3] + boxDiagonalLength;
    } else if (VApex > newUVParameters[3]) {
        // VMax of the cone is lower than that of the Apex.
        newUVParameters[2] = uvParameters[2] - boxDiagonalLength;
        newUVParameters[3] = VApex;
    }
    // Make a new conical face.
    Handle_Geom_Surface aSurface = BRep_Tool::Surface(face);
    TopoDS_Face newFace = BRepBuilderAPI_MakeFace(aSurface, newUVParameters[0], 
        newUVParameters[1], newUVParameters[2], newUVParameters[3], edgeTolerance);
    return newFace;
}
