//McCAD
#include "triangleCollision.hpp"
#include "pointOnSurface.hpp"
#include "distanceEvaluateor.hpp"
#include "tools_impl.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <gp_Pln.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_OBB.hxx>

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
    if (aTriangle.accessMTImpl()->obb.IsOut(obbFace)) return Standard_False;
    // Proceed to check with points of the triangle.
    auto& face = iFace.accessSImpl()->face;
    Standard_Boolean collision = Standard_False;
    Standard_Integer positivePoints = 0;
    Standard_Integer negativePoints = 0;
    auto& points = aTriangle.accessMTImpl()->points;
    for (Standard_Integer i = 0; i <= points.size() - 1; ++i){
        if (PointOnSurface{}(extendedFace, points[i], distanceTolerance)) continue;
        auto evaluate = Tools::DistanceEvaluator{}(face, points[i]);
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
        auto evaluate = Tools::DistanceEvaluator{}(face, points[i]);
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
