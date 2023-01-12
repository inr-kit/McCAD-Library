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
        const double& precision, const double& distanceTolerance) :
    precision{precision}, distanceTolerance{distanceTolerance}{
}

McCAD::Decomposition::TriangleCollision::~TriangleCollision(){
}

/** ********************************************************************
* @brief    An operator that deligates collision detection to specialized functions.
* @param    iFace is a McCAD bound surface object.
* @param    aTriangle is a McCAD mesh triangle object.
* @return   true if collision is detected.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::TriangleCollision::operator()(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::MeshTriangle& aTriangle,
        int& aSide){
    auto surfaceType = iFace.accessSImpl()->surfaceType;
    if (surfaceType == Tools::toTypeName(GeomAbs_Plane)){
        return triangleCollisionPlane(iFace, aTriangle, aSide);
    } else if (surfaceType == Tools::toTypeName(GeomAbs_Cylinder)){
        return triangleCollisionCyl(iFace, aTriangle, aSide);
    } else if (surfaceType == Tools::toTypeName(GeomAbs_Cone)) {
        return triangleCollisionCone(iFace, aTriangle, aSide);
    } else return false;
}

bool
McCAD::Decomposition::TriangleCollision::triangleCollisionPlane(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::MeshTriangle& aTriangle,
        int& aSide){
    const auto & extendedFace = iFace.accessSImpl()->extendedFace;
    // Quick check for collision using OBB
    Bnd_OBB obbFace;
    BRepBndLib::AddOBB(extendedFace, obbFace);
    if (aTriangle.accessMTImpl()->obb.IsOut(obbFace)) return false;
    const auto & face = iFace.accessSImpl()->face;
    bool collision = false;
    int positivePoints{0}, negativePoints{0};
    auto& points = aTriangle.accessMTImpl()->points;
    for (int i = 0; i < points.size(); ++i){
        if (PointOnSurface{}(extendedFace, points[i], distanceTolerance)) continue;
        auto evaluate = Tools::SenseEvaluator{}(face, points[i]);
        if(evaluate){
            if (std::signbit(evaluate.value())) ++negativePoints;
            else ++positivePoints;
        }
        if (positivePoints > 0 && negativePoints > 0){
          collision = true;
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

bool
McCAD::Decomposition::TriangleCollision::triangleCollisionCyl(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::MeshTriangle& aTriangle,
        int& aSide){
    const auto & extendedFace = iFace.accessSImpl()->extendedFace;
    // Quick check for collision using OBB
    Bnd_OBB obbFace;
    BRepBndLib::AddOBB(extendedFace, obbFace);
    if (aTriangle.accessMTImpl()->obb.IsOut(obbFace)) return false;
    // Proceed to check with points of the triangle.
    const auto & face = iFace.accessSImpl()->face;
    bool collision = false;
    int positivePoints{0}, negativePoints{0};
    auto& points = aTriangle.accessMTImpl()->points;
    for (int i = 0; i < points.size(); ++i){
        if (PointOnSurface{}(extendedFace, points[i], distanceTolerance)) continue;
        auto evaluate = Tools::SenseEvaluator{}(face, points[i]);
        if(evaluate){
            if (std::signbit(evaluate.value())) ++negativePoints;
            else ++positivePoints;
        }
        if (positivePoints > 0 && negativePoints > 0){
            collision = true;
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

/** ********************************************************************
* @brief    A function that detects if a mesh triangle collides with a conical surface.
* @param    iFace is a McCAD bound surface object.
* @param    aTriangle is a McCAD mesh triangle object.
* @return   true if collision is detected.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::TriangleCollision::triangleCollisionCone(
    const McCAD::Geometry::BoundSurface& iFace,
    const McCAD::Geometry::MeshTriangle& aTriangle,
    int& aSide) {
    const auto & extendedFace = iFace.accessSImpl()->extendedFace;
    // Quick check for collision using OBB
    Bnd_OBB obbFace;
    BRepBndLib::AddOBB(extendedFace, obbFace);
    if (aTriangle.accessMTImpl()->obb.IsOut(obbFace)) return false;
    // Proceed to check with points of the triangle.
    const auto & face = iFace.accessSImpl()->face;
    bool collision = false;
    int positivePoints{ 0 }, negativePoints{ 0 };
    const auto & points = aTriangle.accessMTImpl()->points;
    for (int i = 0; i < points.size(); ++i) {
        if (PointOnSurface{}(extendedFace, points[i], distanceTolerance)) continue;
        auto evaluate = Tools::SenseEvaluator{}(face, points[i]);
        if (evaluate) {
            if (std::signbit(evaluate.value())) ++negativePoints;
            else ++positivePoints;
        }
        if (positivePoints > 0 && negativePoints > 0) {
            collision = true;
            break;
        }
    }
    if (positivePoints > 0 && negativePoints == 0) {
        aSide = 1;
    }
    else if (positivePoints == 0 && negativePoints > 0) {
        aSide = -1;
    }
    return collision;
}
