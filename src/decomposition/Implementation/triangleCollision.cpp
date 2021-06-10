//McCAD
#include "triangleCollision.hpp"
#include "SurfaceUtilities.hpp"
#include "senseEvaluator.hpp"
#include "pointOnSurface.hpp"
//OCC
#include <Bnd_OBB.hxx>
#include <BRepBndLib.hxx>

Standard_Boolean
McCAD::Decomposition::TriangleCollision::operator()(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::MeshTriangle& aTriangle,
        Standard_Integer& aSide){
    auto surfaceType = iFace.getSurfaceType();
    if (surfaceType == Tools::toTypeName(GeomAbs_Plane)){
        return triangleCollisionPlane(iFace, aTriangle, aSide);
    } else if (surfaceType == Tools::toTypeName(GeomAbs_Cylinder)){
        return triangleCollisionCyl(iFace, aTriangle, aSide);
    }
    else return Standard_False;
}

Standard_Boolean
McCAD::Decomposition::TriangleCollision::triangleCollisionPlane(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::MeshTriangle& aTriangle,
        Standard_Integer& aSide, Standard_Real distanceTolerance,
        Standard_Real senseTolerance){
    //std::cout << "triangleCollision, Plane" << std::endl;
    auto& extendedFace = iFace.accessSImpl()->extendedFace;
    // Quick check for collision using OBB
    Bnd_OBB obbFace;
    BRepBndLib::AddOBB(extendedFace, obbFace);
    if (aTriangle.accessMTImpl()->obb.IsOut(obbFace)) {
        /* //debug
        STEPControl_Writer writer2;
        writer2.Transfer(iFace.accessSImpl()->face, STEPControl_StepModelType::STEPControl_AsIs);
        Bnd_OBB obbFace1;
        BRepBndLib::AddOBB(iFace.accessSImpl()->face, obbFace1);
        writer2.Transfer(SolidSplitter{}.calculateOBB(obbFace1), STEPControl_StepModelType::STEPControl_AsIs);
        writer2.Transfer(aTriangle.accessMTImpl()->face, STEPControl_StepModelType::STEPControl_AsIs);
        writer2.Transfer(SolidSplitter{}.calculateOBB(aTriangle.accessMTImpl()->obb), STEPControl_StepModelType::STEPControl_AsIs);
        Standard_Integer kk = 0;
        std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/t/T";
        std::string suffix = ".stp";
        while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
            ++kk;
        }
        filename += std::to_string(kk);
        filename += suffix;
        writer2.Write(filename.c_str());
        */ //debug
        return Standard_False;
    }
    /* //debug
    STEPControl_Writer writer2;
    writer2.Transfer(iFace.accessSImpl()->face, STEPControl_StepModelType::STEPControl_AsIs);
    Bnd_OBB obbFace1;
    BRepBndLib::AddOBB(iFace.accessSImpl()->face, obbFace1);
    writer2.Transfer(SolidSplitter{}.calculateOBB(obbFace1), STEPControl_StepModelType::STEPControl_AsIs);
    writer2.Transfer(aTriangle.accessMTImpl()->face, STEPControl_StepModelType::STEPControl_AsIs);
    writer2.Transfer(SolidSplitter{}.calculateOBB(aTriangle.accessMTImpl()->obb), STEPControl_StepModelType::STEPControl_AsIs);
    Standard_Integer kk = 0;
    std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/t/T";
    std::string suffix = ".stp";
    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
        ++kk;
    }
    filename += std::to_string(kk);
    filename += suffix;
    writer2.Write(filename.c_str());
    */ //debug
    // Proceed to check with points of the triangle.
    auto& face = iFace.accessSImpl()->face;
    Standard_Boolean collision = Standard_False;
    Standard_Integer positivePoints = 0;
    Standard_Integer negativePoints = 0;
    auto& points = aTriangle.accessMTImpl()->points;
    for (Standard_Integer i = 0; i <= points.size() - 1; ++i){
        if (PointOnSurface{}(extendedFace, points[i], distanceTolerance)) continue;
        auto evaluate = Tools::SenseEvaluator{}(face, points[i]);
        if (std::signbit(evaluate)){
          ++negativePoints;
        } else ++positivePoints;
        if (positivePoints > 0 && negativePoints > 0){
          collision = Standard_True;
          break;
        }
    }
    if (positivePoints > 0 && negativePoints == 0){
        aSide = 1;
    } else if (positivePoints == 0 && negativePoints > 0){
        aSide = -1;
    }
    return collision;
}

Standard_Boolean
McCAD::Decomposition::TriangleCollision::triangleCollisionCyl(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::MeshTriangle& aTriangle,
        Standard_Integer& aSide, Standard_Real distanceTolerance,
        Standard_Real senseTolerance){
    //std::cout << "triangleCollision, Plane" << std::endl;
    auto& extendedFace = iFace.accessSImpl()->extendedFace;
    // Quick check for collision using OBB
    Bnd_OBB obbFace;
    BRepBndLib::AddOBB(extendedFace, obbFace);
    if (aTriangle.accessMTImpl()->obb.IsOut(obbFace)) return Standard_False;
    // Proceed to check with points of the triangle.
    auto& face = iFace.accessSImpl()->face;
    Standard_Boolean collision = Standard_False;
    Standard_Integer positivePoints = 0;
    Standard_Integer negativePoints = 0;
    auto& points = aTriangle.accessMTImpl()->points;
    for (Standard_Integer i = 0; i <= points.size() - 1; ++i){
        if (PointOnSurface{}(extendedFace, points[i], distanceTolerance)) continue;
        auto evaluate = Tools::SenseEvaluator{}(face, points[i]);
        if (evaluate > senseTolerance){
            ++positivePoints;
        } else if (evaluate < senseTolerance){
            ++negativePoints;
        }
        if (positivePoints > 0 && negativePoints > 0){
            collision = Standard_True;
            break;
        }
    }
    if (positivePoints > 0 && negativePoints == 0){
        aSide = 1;
    } else if (positivePoints == 0 && negativePoints > 0){
        aSide = -1;
    }
    return collision;
}
