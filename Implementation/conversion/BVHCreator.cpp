// McCAD
#include "BVHCreator.hpp"
// OC
#include <BRepExtrema_TriangleSet.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

McCAD::Conversion::Impl::BVHCreator::BVHCreator(
        const std::shared_ptr<TopTools_HSequenceOfShape> solidsList){
    for(const auto& aShape : *solidsList){
        TopoDS_Builder aBuilder;
        TopoDS_Compound aCompound;
        BRepExtrema_ShapeList aFaceList;
        for (TopExp_Explorer i(aShape, TopAbs_FACE); i.More(); i.Next()){
            aFaceList.Append(TopoDS::Face(i.Current()));
        }
        std::cout << "aFaceList" << std::endl;
        //aBuilder.MakeCompound(aCompound);
        BRepMesh_IncrementalMesh aMesher(aShape, 1.0);
        BRepExtrema_TriangleSet aTriangleSet(aFaceList);
        const auto& aBvh = aTriangleSet.BVH();
        for (int i = 0; i < aBvh->Length(); i++){
            const BVH_Vec4i& aNodeData = aBvh->NodeInfoBuffer()[i];
            std::cout << "BVH_Vec4i& aNodeData" << std::endl;
            if (aNodeData.x() == 0){
                // inner node.
                std::cout << "inner node" << std::endl;
                const BVH_Vec3d& aP1 = aBvh->MinPoint(aNodeData.y());
                const BVH_Vec3d& aP2 = aBvh->MaxPoint(aNodeData.y());
                const BVH_Vec3d& aQ1 = aBvh->MinPoint(aNodeData.z());
                const BVH_Vec3d& aQ2 = aBvh->MaxPoint(aNodeData.z());
                try{
                    std::cout << "inner node try" << std::endl;
                    BRepPrimAPI_MakeBox aBoxMaker(gp_Pnt(aP1.x(), aP1.y(), aP1.z()),
                    gp_Pnt(aP2.x(), aP2.y(), aP2.z()));
                    std::cout << "inner node aBoxMaker" << std::endl;
                    for (TopExp_Explorer i(aBoxMaker.Shape(), TopAbs_EDGE);
                         i.More(); i.Next()){
                        aBuilder.Add(aCompound, i.Current());
                        std::cout << "inner node aBuilder.Add" << std::endl;
                    }
                } catch (Standard_Failure f) {}
                try{
                    std::cout << "inner node try2" << std::endl;
                    BRepPrimAPI_MakeBox aBoxMaker(gp_Pnt(aQ1.x(), aQ1.y(), aQ1.z()),
                    gp_Pnt(aQ2.x(), aQ2.y(), aQ2.z()));
                    for (TopExp_Explorer i(aBoxMaker.Shape(), TopAbs_EDGE);
                         i.More(); i.Next()){
                        aBuilder.Add(aCompound, i.Current());
                    }
                } catch (Standard_Failure f) {}
            } else{
                // leaf node.
                std::cout << "leaf node" << std::endl;
                const BVH_Vec3d& aP1 = aBvh->MinPoint(i);
                const BVH_Vec3d& aP2 = aBvh->MaxPoint(i);
                try{
                    std::cout << "leaf node try" << std::endl;
                    BRepPrimAPI_MakeBox aBoxMaker(gp_Pnt(aP1.x(), aP1.y(), aP1.z()),
                    gp_Pnt(aP2.x(), aP2.y(), aP2.z()));
                    aBuilder.Add(aCompound, aBoxMaker.Shape());
                } catch (Standard_Failure f) {}
            }
        }
        std::cout << "write" << std::endl;
        BRepTools::Write(aCompound, "bvh.brep");
    }
}

McCAD::Conversion::Impl::BVHCreator::~BVHCreator(){}
