//McCAD
#include "faceCollision.hpp"
#include "triangleCollision.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <BRepTools.hxx>

Standard_Boolean
McCAD::Decomposition::FaceCollision::operator()(const BoundSurface& aFace,
                                                Standard_Integer& aSide){
    auto surfaceType = aFace.accessSImpl()->getType();
    if (surfaceType == "Plane"){
        return faceCollisionPlane(aFace, aSide);
    } else if (surfaceType == "Cylinder"){
        //return faceCollisionCylinder(aSide);
    }
    return Standard_False;
}

Standard_Boolean
McCAD::Decomposition::FaceCollision::faceCollisionPlane(const BoundSurface& aFace,
                                                        Standard_Integer& aSide){
    Standard_Boolean collision = Standard_False;
    Standard_Integer positiveTriangles = 0;
    Standard_Integer negativeTriangles = 0;
    auto& meshTriangleList = aFace.accessBSImpl()->meshTrianglesList;
    for (Standard_Integer i = 0; i <= meshTriangleList.size() - 1; ++i){
        Standard_Integer side = 0;
        if (triangleCollision<Geometry::BoundSurfacePlane>(
                    aFace.accessSImpl()->extendedFace, *(meshTriangleList[i]),
                    side)){
            collision = Standard_True;
            break;
        } else{
            if (side == 1){
                ++positiveTriangles;
            } else if (side == -1){
                ++negativeTriangles;
            }
        }
        if (positiveTriangles > 0 && negativeTriangles > 0){
            collision = Standard_True;
            break;
        }
    }
    // Update side.
    if (positiveTriangles > 0 && negativeTriangles == 0){
        aSide = 1;
    } else if (positiveTriangles == 0 && negativeTriangles > 0){
        aSide = -1;
    }
    return collision;
}
