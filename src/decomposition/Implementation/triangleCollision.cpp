//McCAD
#include "triangleCollision.hpp"
#include "SurfaceUtilities.hpp"
#include "senseEvaluator.hpp"
#include "pointOnSurface.hpp"
//OCC
#include <Bnd_OBB.hxx>
#include <BRepBndLib.hxx>

McCAD::Decomposition::TriangleCollision::TriangleCollision(){
}

McCAD::Decomposition::TriangleCollision::TriangleCollision(
        const Standard_Real& precision, const Standard_Real& distanceTolerance) :
    precision{precision}, distanceTolerance{distanceTolerance}{
}

McCAD::Decomposition::TriangleCollision::~TriangleCollision(){
}

Standard_Boolean
McCAD::Decomposition::TriangleCollision::operator()(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::MeshTriangle& aTriangle,
        Standard_Integer& aSide){
    auto surfaceType = iFace.accessSImpl()->surfaceType;
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
        Standard_Integer& aSide){
    auto& extendedFace = iFace.accessSImpl()->extendedFace;
    // Quick check for collision using OBB
    Bnd_OBB obbFace;
    BRepBndLib::AddOBB(extendedFace, obbFace);
    if (aTriangle.accessMTImpl()->obb.IsOut(obbFace)) return Standard_False;
    auto& face = iFace.accessSImpl()->face;
    Standard_Boolean collision = Standard_False;
    Standard_Integer positivePoints{0}, negativePoints{0};
    auto& points = aTriangle.accessMTImpl()->points;
    for (Standard_Integer i = 0; i < points.size(); ++i){
        if (PointOnSurface{}(extendedFace, points[i], distanceTolerance)) continue;
        auto evaluate = Tools::SenseEvaluator{}(face, points[i]);
        if(evaluate){
            if (std::signbit(evaluate.value())) ++negativePoints;
            else ++positivePoints;
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

Standard_Boolean
McCAD::Decomposition::TriangleCollision::triangleCollisionCyl(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::MeshTriangle& aTriangle,
        Standard_Integer& aSide){
    auto& extendedFace = iFace.accessSImpl()->extendedFace;
    // Quick check for collision using OBB
    Bnd_OBB obbFace;
    BRepBndLib::AddOBB(extendedFace, obbFace);
    if (aTriangle.accessMTImpl()->obb.IsOut(obbFace)) return Standard_False;
    // Proceed to check with points of the triangle.
    auto& face = iFace.accessSImpl()->face;
    Standard_Boolean collision = Standard_False;
    Standard_Integer positivePoints{0}, negativePoints{0};
    auto& points = aTriangle.accessMTImpl()->points;
    for (Standard_Integer i = 0; i < points.size(); ++i){
        if (PointOnSurface{}(extendedFace, points[i], distanceTolerance)) continue;
        auto evaluate = Tools::SenseEvaluator{}(face, points[i]);
        if(evaluate){
            if (std::signbit(evaluate.value())) ++negativePoints;
            else ++positivePoints;
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
