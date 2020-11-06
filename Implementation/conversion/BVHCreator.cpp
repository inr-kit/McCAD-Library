// McCAD
#include "BVHCreator.hpp"
// OC
#include <STEPControl_Writer.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Face.hxx>
#include <BVH_PrimitiveSet3d.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepExtrema_TriangleSet.hxx>
#include <BVH_Tree.hxx>
#include <BVH_Box.hxx>
#include <BVH_Constants.hxx>
#include <BVH_LinearBuilder.hxx>
#include <Standard_Transient.hxx>
#include <NCollection_Handle.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

McCAD::Conversion::Impl::BVHCreator::BVHCreator(
        const std::shared_ptr<TopTools_HSequenceOfShape> solidsList){
    STEPControl_Writer writer;
    BRepExtrema_ShapeList aFaceList;
    for(const auto& aShape : *solidsList){
        BRepMesh_IncrementalMesh aMesher(aShape, 1.0);
        //BRepMesh_IncrementalMesh incMesh(face, meshDeflection, Standard_True);
        for (TopExp_Explorer i(aShape, TopAbs_FACE); i.More(); i.Next()){
            aFaceList.Append(TopoDS::Face(i.Current()));
        }
    }
    std::cout << "aFaceList: " << aFaceList.Size() << std::endl;
    BRepExtrema_TriangleSet aTriangleSet(aFaceList);
    std::cout << "aTriangleSet: " << aTriangleSet.Size() << std::endl;
    const opencascade::handle<BVH_LinearBuilder<Standard_Real, 3>> bvhLBuilder;
    aTriangleSet.SetBuilder(bvhLBuilder);
    const opencascade::handle<BVH_Tree<Standard_Real, 3, BVH_BinaryTree> >&
            aBvh = aTriangleSet.BVH();
    std::cout << "Length: " << aBvh->Length() << std::endl;
    std::cout << "Depth: " << aBvh->Depth() << std::endl;
    BVH_Box<double, 3> AABB = aTriangleSet.Box();
    auto& CornerMin = AABB.CornerMin();
    auto& CornerMax = AABB.CornerMax();
    BRepPrimAPI_MakeBox aBoxMaker(gp_Pnt(CornerMin.x(), CornerMin.y(), CornerMin.z()),
                                  gp_Pnt(CornerMax.x(), CornerMax.y(), CornerMax.z()));
    writer.Transfer(aBoxMaker.Shape(), STEPControl_StepModelType::STEPControl_AsIs);
    const BVH_Array4i& aNodeData = aBvh->NodeInfoBuffer();
    for (int i = 0; i < aBvh->Length(); ++i){
        auto& Min = aBvh->MinPoint(i);
        auto& Max = aBvh->MaxPoint(i);
        BRepPrimAPI_MakeBox aBoxMaker(gp_Pnt(Min.x(), Min.y(), Min.z()),
                                      gp_Pnt(Max.x(), Max.y(), Max.z()));
        writer.Transfer(aBoxMaker.Shape(), STEPControl_StepModelType::STEPControl_AsIs);
        std::cout << "Node ID: " << i <<
                     ", NbPrimitives: " << aBvh->NbPrimitives(i) <<
                     ", IsOuter: " << aBvh->IsOuter(i) <<
                     ", BegPrimitivet: " << aBvh->BegPrimitive(i) <<
                     ", EndPrimitive : " << aBvh->EndPrimitive (i) <<
                     ", Level: " << aBvh->Level(i) <<
                     std::endl;
    }
    std::cout << "write" << std::endl;
    writer.Write("bvh.step");
}

McCAD::Conversion::Impl::BVHCreator::~BVHCreator(){}
