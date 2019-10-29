//McCAD
#include "faceCollision.hpp"
#include "triangleCollision.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <BRepTools.hxx>

Standard_Boolean
McCAD::Decomposition::FaceCollision::operator()(
        const McCAD::Geometry::BoundSurface& iFace,
        const McCAD::Geometry::BoundSurface& jFace,
        Standard_Integer& aSide){
    auto surfaceType = iFace.getSurfaceType();
    if (surfaceType == "Plane"){
        auto& face = iFace.accessSImpl()->face;
        auto& meshTriangleList = jFace.accessBSImpl()->meshTrianglesList;
        return faceCollisionPlane(face, meshTriangleList, aSide);
    } else if (surfaceType == "Cylinder"){
        //return faceCollisionCylinder(iFace, , jFace, aSide);
    }
    return Standard_False;
}

Standard_Boolean
McCAD::Decomposition::FaceCollision::faceCollisionPlane(
        const TopoDS_Face& face,
        const std::vector<std::unique_ptr<McCAD::Geometry::MeshTriangle>>& meshTriangleList,
        Standard_Integer& aSide){
    Standard_Boolean collision = Standard_False;
    Standard_Integer positiveTriangles = 0;
    Standard_Integer negativeTriangles = 0;
    for (Standard_Integer i = 0; i <= meshTriangleList.size() - 1; ++i){
        Standard_Integer side = 0;
        if (TriangleCollision{}.triangleCollision<Geometry::BoundSurfacePlane>(
                    face, *(meshTriangleList[i]), side)){
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
