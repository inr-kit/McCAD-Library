// C++
#include <optional>
#include <filesystem>
// McCAD
#include "splitSurfacesGenerator.hpp"
#include "SurfaceUtilities.hpp"
#include "faceParameters.hpp"
#include "extendedFaceCreator.hpp"
#include "CurveUtilities.hpp"
// OCC
#include <GeomAbs_SurfaceType.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
#include <BRepFill.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <STEPControl_Writer.hxx>

McCAD::Decomposition::SplitSurfaceGenerator::SplitSurfaceGenerator(){
}

McCAD::Decomposition::SplitSurfaceGenerator::SplitSurfaceGenerator(
        const Standard_Real& edgeTolerance, const Standard_Real& precision,
        const Standard_Real& angularTolerance) :
    edgeTolerance{edgeTolerance}, precision{precision}, angularTolerance{angularTolerance}{
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
    gp_Dir splitSurfNormal;
    if(firstNormal.Angle(secondNormal) < angularTolerance){
        // Use edge and one of the normals to get surface normal.
        gp_Vec edgeVector{edgeStart, edgeEnd};
        gp_Dir edgeDir{edgeVector};
        splitSurfNormal = edgeDir.Crossed(firstNormal);
    } else{
        // Directions are normalized. Create a direction at half the angle between the directions.
        gp_Dir midwayNormal;
        midwayNormal.SetCoord((firstNormal.X() + secondNormal.X())/2.0,
                              (firstNormal.Y() + secondNormal.Y())/2.0,
                              (firstNormal.Z() + secondNormal.Z())/2.0);
        splitSurfNormal = midwayNormal.Crossed(firstNormal.Crossed(secondNormal));
    }
    gp_Pln splitSurf(splitSurfPoint, splitSurfNormal);
    return BRepBuilderAPI_MakeFace(splitSurf).Face();
}

std::optional<TopoDS_Face>
McCAD::Decomposition::SplitSurfaceGenerator::generatePlaneOnCurve(
        const std::shared_ptr<Geometry::Edge>& edge){
    gp_Dir splitSurfNormal;
    gp_Pnt splitSurfPoint{edge->accessEImpl()->startPoint};
    try{
        if(edge->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Circle)){
            gp_Circ circle = BRepAdaptor_Curve(edge->accessEImpl()->edge).Circle();
            splitSurfNormal = circle.Axis().Direction();
            splitSurfPoint = circle.Location();
        } else if (edge->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Ellipse)){
            gp_Elips ellipse = BRepAdaptor_Curve(edge->accessEImpl()->edge).Ellipse();
            splitSurfNormal = ellipse.Axis().Direction();
            splitSurfPoint = ellipse.Location();
        } else if (edge->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Hyperbola)){
            gp_Hypr hyperbola = BRepAdaptor_Curve(edge->accessEImpl()->edge).Hyperbola();
            splitSurfNormal = hyperbola.Axis().Direction();
            splitSurfPoint = hyperbola.Location();
        } else if (edge->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Parabola)){
            gp_Parab parabola = BRepAdaptor_Curve(edge->accessEImpl()->edge).Parabola();
            splitSurfNormal = parabola.Axis().Direction();
            splitSurfPoint = parabola.Location();
        }
    } catch(...){
        gp_Pnt edgeStart{edge->accessEImpl()->startPoint}, edgeEnd{edge->accessEImpl()->endPoint},
               edgeMid{edge->accessEImpl()->middlePoint}, edgeExtra{edge->accessEImpl()->extraPoint};
        gp_Dir firstDir, secondDir;
        if(edgeStart.IsEqual(edgeEnd, precision)){
            // Curve is closed.
            gp_Vec firstVec(edgeStart, edgeMid), secondVec(edgeExtra, edgeMid);
            gp_Dir dir1(firstVec);
            firstDir = dir1;
            gp_Dir dir2(secondVec);
            secondDir = dir2;
        } else {
            gp_Vec firstVec(edgeStart, edgeMid), secondVec(edgeEnd, edgeMid);
            gp_Dir dir1(firstVec);
            firstDir = dir1;
            gp_Dir dir2(secondVec);
            secondDir = dir2;
        }
        splitSurfNormal = firstDir.Crossed(secondDir);
    }
    gp_Pln splitSurf(splitSurfPoint, splitSurfNormal);
    return BRepBuilderAPI_MakeFace(splitSurf).Face();
}

std::optional<TopoDS_Face>
McCAD::Decomposition::SplitSurfaceGenerator::generateSurfOnBSpline(
        const TopoDS_Face& firstFace, const TopoDS_Face& secondFace,
        const std::shared_ptr<Geometry::Edge>& edge){
    BRepAdaptor_Curve firstAdaptor(edge->accessEImpl()->edge, firstFace),
                      secondAdaptor(edge->accessEImpl()->edge, secondFace);
    if(firstAdaptor.IsCurveOnSurface() && !firstAdaptor.IsClosed()){
        // The B-Spline curve is not on the extremities of the cylinder.
    } else if(secondAdaptor.IsCurveOnSurface() && !secondAdaptor.IsClosed()){
        // The B-Spline curve is not on the extremities of the cylinder.
    }
    return std::nullopt;
}


std::optional<TopoDS_Face>
McCAD::Decomposition::SplitSurfaceGenerator::generatePlaneOn2Lines(
        const std::shared_ptr<Geometry::Edge>& firstEdge,
        const std::shared_ptr<Geometry::Edge>& secondEdge){
    gp_Pnt firstEdgeStart{firstEdge->accessEImpl()->startPoint},
           firstEdgeEnd{firstEdge->accessEImpl()->endPoint},
           secondEdgeMid{secondEdge->accessEImpl()->middlePoint};
    gp_Vec firstVec(firstEdgeStart, secondEdgeMid),
           secondVec(firstEdgeEnd, secondEdgeMid);
    gp_Dir firstDir(firstVec), secondDir(secondVec);
    gp_Dir splitSurfNormal = firstDir.Crossed(secondDir);
    gp_Pln splitSurf(secondEdgeMid, splitSurfNormal);
    return BRepBuilderAPI_MakeFace(splitSurf).Face();
}

std::optional<TopoDS_Face>
McCAD::Decomposition::SplitSurfaceGenerator::generatePlaneOnLineAxis(
        const gp_Cylinder& cylinder,
        const std::shared_ptr<Geometry::Edge>& edge){
    gp_Pnt edgeStart{edge->accessEImpl()->startPoint},
           edgeMid{edge->accessEImpl()->middlePoint},
           edgeEnd{edge->accessEImpl()->endPoint},
           cylinderOrigin{cylinder.Location()};
    gp_Vec firstVec(edgeStart, cylinderOrigin),
           secondVec(edgeEnd, cylinderOrigin);
    gp_Dir firstDir(firstVec), secondDir(secondVec);
    gp_Dir splitSurfNormal = firstDir.Crossed(secondDir);
    gp_Pln splitSurf(edgeMid, splitSurfNormal);
    return BRepBuilderAPI_MakeFace(splitSurf).Face();
}
