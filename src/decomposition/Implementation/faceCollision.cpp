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
        const double& precision, const double& distanceTolerance) :
    precision{precision}, distanceTolerance{distanceTolerance}{
}

McCAD::Decomposition::FaceCollision::~FaceCollision(){
}

/** ********************************************************************
* @brief    An operator that calls on a function to perform collision detection.
* @param    firstFace is a McCAD bound surface object.
* @param    secondFace is a McCAD bound surface object.
* @return   true if collision is detected between the two input surfaces.
* @date     31/12/2020
* @modified 
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::FaceCollision::operator()(
        const McCAD::Geometry::BoundSurface& firstFace,
        const McCAD::Geometry::BoundSurface& secondFace,
        int& aSide){
    return faceCollision(firstFace, secondFace.accessBSImpl()->meshTrianglesList,
                         aSide);
}

/** ********************************************************************
* @brief    A function that performs collision detection between a surface and surface facets.
* @param    firstFace is a McCAD bound surface object.
* @param    meshTriangleList is a list of McCAD mesh triangle objects.
* @return   true if collision is detected.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::FaceCollision::faceCollision(
        const McCAD::Geometry::BoundSurface& firstFace,
        const std::vector<std::shared_ptr<McCAD::Geometry::MeshTriangle>>& meshTriangleList,
        int& aSide){
    bool collision = false;
    int positiveTriangles{0}, negativeTriangles{0};
    for (int i = 0; i < meshTriangleList.size(); ++i){
        int side{0};
        if (TriangleCollision{precision, distanceTolerance}(
                    firstFace, *(meshTriangleList[i]), side)){
            collision = true;
            break;
        } else{
            if (side == 1) ++positiveTriangles;
            else if (side == -1) ++negativeTriangles;
        }
        if (positiveTriangles > 0 && negativeTriangles > 0){
            collision = true;
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
