//McCAD
#include "faceCollision.hpp"
#include "triangleCollision.hpp"
#include "tools_impl.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <BRepTools.hxx>

McCAD::Decomposition::FaceCollision::FaceCollision(){
}

McCAD::Decomposition::FaceCollision::FaceCollision(
        const Standard_Real& precision, const Standard_Real& distanceTolerance) :
    precision{precision}, distanceTolerance{distanceTolerance}{
}

McCAD::Decomposition::FaceCollision::~FaceCollision(){
}

Standard_Boolean
McCAD::Decomposition::FaceCollision::operator()(
        const McCAD::Geometry::BoundSurface& firstFace,
        const McCAD::Geometry::BoundSurface& secondFace,
        Standard_Integer& aSide){
    return faceCollision(firstFace, secondFace.accessBSImpl()->meshTrianglesList,
                         aSide);
}

Standard_Boolean
McCAD::Decomposition::FaceCollision::faceCollision(
        const McCAD::Geometry::BoundSurface& firstFace,
        const std::vector<std::shared_ptr<McCAD::Geometry::MeshTriangle>>& meshTriangleList,
        Standard_Integer& aSide){
    auto& face = firstFace.accessSImpl()->face;
    Standard_Boolean collision = Standard_False;
    Standard_Integer positiveTriangles{0}, negativeTriangles{0};
    for (Standard_Integer i = 0; i < meshTriangleList.size(); ++i){
        Standard_Integer side{0};
        if (TriangleCollision{precision, distanceTolerance}(
                    firstFace, *(meshTriangleList[i]), side)){
            collision = Standard_True;
            break;
        } else{
            if (side == 1) ++positiveTriangles;
            else if (side == -1) ++negativeTriangles;
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
