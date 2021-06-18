// C++
#include <optional>
// McCAD
#include "splitSurfacesGenerator.hpp"
#include "SurfaceUtilities.hpp"
#include "faceParameters.hpp"
#include "extendedFaceCreator.hpp"
// OCC
#include <GeomAbs_SurfaceType.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

McCAD::Decomposition::SplitSurfaceGenerator::SplitSurfaceGenerator(){
}

McCAD::Decomposition::SplitSurfaceGenerator::SplitSurfaceGenerator(
        const Standard_Real& edgeTolerance) : edgeTolerance{edgeTolerance}{
}

McCAD::Decomposition::SplitSurfaceGenerator::~SplitSurfaceGenerator(){
}

std::optional<TopoDS_Face>
McCAD::Decomposition::SplitSurfaceGenerator::generatePlaneOnLine(
        const TopoDS_Face& firstFace, const TopoDS_Face& secondFace,
        const std::shared_ptr<Geometry::Edge>& edge){
    gp_Pnt edgeStart{edge->accessEImpl()->startPoint},
           edgeEnd{edge->accessEImpl()->endPoint}, splitSurfPoint{edge->accessEImpl()->startPoint};
    // Try with the start point first then the end.
    std::optional<gp_Dir> first = Tools::FaceParameters{}.normalOnFace(firstFace, edgeStart);
    std::optional<gp_Dir> second = Tools::FaceParameters{}.normalOnFace(secondFace, edgeStart);
    if (!first || !second){
        splitSurfPoint = edgeEnd;
        first = Tools::FaceParameters{}.normalOnFace(firstFace, edgeEnd);
        second = Tools::FaceParameters{}.normalOnFace(secondFace, edgeEnd);
        if (!first || !second) return std::nullopt;
    }
    gp_Dir firstNormal{first.value()}, secondNormal{second.value()};
    // Directions are normalized. Create a direction at half the angle between the directions.
    gp_Dir midwayNormal;
    midwayNormal.SetCoord((firstNormal.X() + secondNormal.X())/2.0,
                          (firstNormal.Y() + secondNormal.Y())/2.0,
                          (firstNormal.Z() + secondNormal.Z())/2.0);
    gp_Dir splitSurfNormal = midwayNormal.Crossed(firstNormal.Crossed(secondNormal));
    gp_Pln splitSurf(splitSurfPoint, splitSurfNormal);
    return BRepBuilderAPI_MakeFace(splitSurf);
}

std::optional<TopoDS_Face>
McCAD::Decomposition::SplitSurfaceGenerator::generatePlaneOn2Lines(
        const std::shared_ptr<Geometry::Edge>& firstEedge,
        const std::shared_ptr<Geometry::Edge>& secondEdge){
    gp_Pnt firstEdgeStart{firstEedge->accessEImpl()->startPoint},
           firstEdgeEnd{firstEedge->accessEImpl()->endPoint},
           secondEdgeMid{secondEdge->accessEImpl()->middlePoint};
    gp_Vec firstVec(firstEdgeStart, secondEdgeMid),
           secondVec(firstEdgeEnd, secondEdgeMid);
    gp_Dir firstDir(firstVec), secondDir(secondVec);
    gp_Dir splitSurfNormal = firstDir.Crossed(secondDir);
    gp_Pln splitSurf(secondEdgeMid, splitSurfNormal);
    return BRepBuilderAPI_MakeFace(splitSurf);
}

std::optional<TopoDS_Face>
McCAD::Decomposition::SplitSurfaceGenerator::generatePlaneOnCurve(
        const TopoDS_Face& firstFace, const TopoDS_Face& secondFace,
        std::shared_ptr<Geometry::Edge> edge){
    return std::nullopt;
}

