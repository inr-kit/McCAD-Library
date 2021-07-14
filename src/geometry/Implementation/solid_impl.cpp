// C++
#include <filesystem>
#include <algorithm>
#include <optional>
// McCAD
#include "solid_impl.hpp"
#include "faceParameters.hpp"
// OCC
#include <TopoDS.hxx>
#include <BRepBndLib.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopExp.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <Geom_Curve.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRep_Tool.hxx>

McCAD::Geometry::Solid::Impl::Impl() :
    splitSolidList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
}

McCAD::Geometry::Solid::Impl::~Impl(){
}

void
McCAD::Geometry::Solid::Impl::initiate(const TopoDS_Shape& aSolidShape){
    solidShape = aSolidShape;
    solid = TopoDS::Solid(solidShape);
}

void
McCAD::Geometry::Solid::Impl::repairSolid(Standard_Real precision,
                                          Standard_Real faceTolerance){
    Tools::Preprocessor preproc{precision, faceTolerance};
    preproc.accessImpl()->removeSmallFaces(solidShape);
    solid = TopoDS::Solid(solidShape);
    preproc.accessImpl()->repairSolid(solid);
}

void
McCAD::Geometry::Solid::Impl::createBB(Standard_Real bndBoxGap){
    // Build OBB
    BRepBndLib::AddOBB(solid, obb);
    obb.Enlarge(bndBoxGap);
    // Build AABB
    BRepBndLib::Add(solid, aabb);
    aabb.Enlarge(bndBoxGap);
}

void
McCAD::Geometry::Solid::Impl::calcMeshDeflection(Standard_Real scalingFactor){
    meshDeflection = 2 * std::min({obb.XHSize(), obb.YHSize(), obb.ZHSize()}) /
            scalingFactor;
    // error in Bnd_OBB.hxx. calculate it till the method is fixed
    // boxDiagonalLength = sqrt(obb.SquareExtent());
    boxDiagonalLength = 2 * sqrt(std::pow(obb.XHSize(), 2) +
                                 std::pow(obb.YHSize(), 2) +
                                 std::pow(obb.ZHSize(), 2));
}

void
McCAD::Geometry::Solid::Impl::updateEdgesConvexity(Standard_Real angularTolerance,
                                                   Standard_Real precision){
    TopTools_IndexedDataMapOfShapeListOfShape mapEdgeFace;
    TopExp::MapShapesAndAncestors(solid, TopAbs_EDGE, TopAbs_FACE, mapEdgeFace);
    TopTools_ListOfShape facesList;
    for (Standard_Integer edgeNumber = 1; edgeNumber <= mapEdgeFace.Extent();
         ++edgeNumber){
        TopoDS_Edge edge = TopoDS::Edge(mapEdgeFace.FindKey(edgeNumber));
        BRepAdaptor_Curve curveAdaptor;
        curveAdaptor.Initialize(edge);
        facesList = mapEdgeFace.FindFromKey(edge);
        if(facesList.Extent() != Standard_Integer(2)){
            // Ignore edge
            edge.Convex(3);
            continue;
        }
        TopTools_ListIteratorOfListOfShape iterFace(facesList);
        TopoDS_Face firstFace = TopoDS::Face(iterFace.Value());
        iterFace.Next();
        TopoDS_Face secondFace = TopoDS::Face(iterFace.Value());

        Standard_Real start, end;
        Handle_Geom_Curve curve = BRep_Tool::Curve(edge, start, end);
        gp_Pnt startPoint;
        gp_Vec vector;
        curve->D1(start, startPoint, vector);
        gp_Dir direction(vector);

        // Get the normals of each surface
        std::optional<gp_Dir> firstNormal = Tools::FaceParameters{
                precision}.normalOnFace(firstFace, startPoint);
        std::optional<gp_Dir> secondNormal = Tools::FaceParameters{
                precision}.normalOnFace(secondFace, startPoint);
        if(!firstNormal || !secondNormal){
            // Ignore edge
            edge.Convex(3);
            continue;
        }
        Standard_Real angle = (*firstNormal).AngleWithRef(*secondNormal, direction);
        if(std::abs(angle) < angularTolerance){
            angle = Standard_Real(0);
        }
        // The edge is convex.
        if( angle < Standard_Real(0) && edge.Orientation() == TopAbs_REVERSED){
            edge.Convex(1);
        } else if(angle > Standard_Real(0) && edge.Orientation() == TopAbs_FORWARD){
            edge.Convex(1);
        } else if (angle == Standard_Real(0)){
            // edge is flat
            edge.Convex(2);
        } else{
            // edge is concave
            edge.Convex(0);
        }
    }
}

void
McCAD::Geometry::Solid::Impl::calcAABBCenter(){
    Standard_Real minX, minY, minZ, maxX, maxY, maxZ;
    aabb.Get(minX, minY, minZ, maxX, maxY, maxZ);
    aabbCenter = {(minX + std::abs(maxX-minX)/2.0),
                  (minY + std::abs(maxY-minY)/2.0),
                  (minZ + std::abs(maxZ-minZ)/2.0)};
}

void
McCAD::Geometry::Solid::Impl::calcVolume(){
    // Volume in mm3
    GProp_GProps geometryProperties;
    BRepGProp::VolumeProperties(solid, geometryProperties);
    solidVolume = geometryProperties.Mass();
}
