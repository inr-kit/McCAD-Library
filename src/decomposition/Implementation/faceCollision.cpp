//McCAD
#include "faceCollision.hpp"
#include "triangleCollision.hpp"
#include "tools_impl.hpp"
#include "senseEvaluator.hpp"
#include "pointOnSurface.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBndLib.hxx>

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
        // Standard_Boolean vertexJudge = Standard_False;
        //     if(!faceCollision(firstFace, secondFace.accessBSImpl()->meshTrianglesList,
        //                  aSide))
        //                 {
        //                     std::cout<<"---------------------------\n";
        //                     Standard_Integer positivevertex{0},negativevertex{0};
        //                     auto& extendedFace = firstFace.accessSImpl()->extendedFace;
        //                     auto& firstface = firstFace.accessSImpl()->face;
        //                     auto& secondface = secondFace.accessSImpl()->face;
        //                     std::vector<gp_Pnt> pointslist;
        //                     for(TopExp_Explorer ex(secondface,TopAbs_VERTEX);ex.More();ex.Next())
        //                     {
        //                         TopoDS_Vertex vertex = TopoDS::Vertex(ex.Current());
        //                         pointslist.push_back(BRep_Tool::Pnt(vertex));
        //                     }
        //                     std::cout<<"there are "<<pointslist.size()<<" points on surface.\n";
        //                     for(Standard_Integer i = 0;i<pointslist.size();i++)
        //                     {
        //                         std::cout<<pointslist[i].X()<<" "<<pointslist[i].Y()<<" "<<pointslist[i].Z()<<std::endl;
        //                         if (PointOnSurface{}(extendedFace, pointslist[i], 1e-6)) continue;
        //                         auto evaluate = Tools::SenseEvaluator{}(firstface, pointslist[i]);
        //                         std::cout<<"evaluate = "<<evaluate.value()<<std::endl;
        //                         if(evaluate)
        //                         {
        //                             if (std::signbit(evaluate.value())) ++negativevertex;
        //                             else ++positivevertex;
        //                         }
        //                         if (positivevertex > 0 && negativevertex > 0)
        //                         {
        //                             std::cout<<"positive: "<<positivevertex<<" and negative: "<<negativevertex<<std::endl;
        //                             vertexJudge = Standard_True;
        //                             break;
        //                         }
        //                    }
        //                 }
        //                 return vertexJudge;
}

Standard_Boolean
McCAD::Decomposition::FaceCollision::faceCollision(
        const McCAD::Geometry::BoundSurface& firstFace,
        const std::vector<std::shared_ptr<McCAD::Geometry::MeshTriangle>>& meshTriangleList,
        Standard_Integer& aSide){
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
