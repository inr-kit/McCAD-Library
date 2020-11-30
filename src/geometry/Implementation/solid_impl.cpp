// McCAD
#include "solid_impl.hpp"

McCAD::Geometry::Solid::Impl::Impl()
  : preproc{std::make_unique<Tools::Preprocessor>()},
    splitSolidList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
}

McCAD::Geometry::Solid::Impl::~Impl(){
}

void
McCAD::Geometry::Solid::Impl::initiate(const TopoDS_Shape& aSolidShape){
    //std::cout << "initiate" << std::endl;
    solidShape = aSolidShape;
    solid = TopoDS::Solid(solidShape);
}

void
McCAD::Geometry::Solid::Impl::repairSolid(){
    preproc->accessImpl()->removeSmallFaces(solidShape);
    solid = TopoDS::Solid(solidShape);
    preproc->accessImpl()->repairSolid(solid);
}

void
McCAD::Geometry::Solid::Impl::createOBB(Standard_Real bndBoxGap){
    BRepBndLib::AddOBB(solid, obb);
    obb.Enlarge(bndBoxGap);
}

void
McCAD::Geometry::Solid::Impl::calcMeshDeflection(Standard_Real scalingFactor){
    // Calculate the oriented bounding box of the solid.
    meshDeflection = 2 * std::max({obb.XHSize(), obb.YHSize(), obb.ZHSize()}) /
            scalingFactor;
    // error in Bnd_OBB.hxx. calculate it till the method is fixed
    // boxDiagonalLength = sqrt(obb.SquareExtent());
    boxDiagonalLength = 2 * sqrt(std::pow(obb.XHSize(), 2) +
                                 std::pow(obb.YHSize(), 2) +
                                 std::pow(obb.ZHSize(), 2));
}

void
McCAD::Geometry::Solid::Impl::updateEdgesConvexity(const Standard_Real& angleTolerance){
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
        gp_Dir firstNormal = Tools::normalOnFace(firstFace, startPoint);
        gp_Dir secondNormal = Tools::normalOnFace(secondFace, startPoint);
        Standard_Real angle = firstNormal.AngleWithRef(secondNormal, direction);

        if(std::abs(angle) < angleTolerance){
            angle = Standard_Real(0);
        }
        // The edge is convex.
        if( angle < Standard_Real(0) && edge.Orientation() == TopAbs_REVERSED){
            edge.Convex(1);
        } else if(angle > Standard_Real(0) && edge.Orientation() == TopAbs_FORWARD){
            edge.Convex(1);
        } else if (angle == Standard_Real(0)){
            // edge if flat
            edge.Convex(2);
        } else{
            // edge is concave
            edge.Convex(0);
        }
    }
}
