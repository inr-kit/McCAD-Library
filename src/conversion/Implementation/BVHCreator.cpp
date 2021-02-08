// McCAD
#include "BVHCreator.hpp"
// OCC
#include <BRepExtrema_TriangleSet.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <STEPControl_Writer.hxx>
// MOAB
//#include <MBEntityType.h>
//#include <MBTagConventions.hpp>
//#include <moab/ReaderWriterSet.hpp>
// //#include <moab/ReaderIface.hpp>
// //#include "moab/CartVect.hpp"
// //#include "moab/Range.hpp"
//#include "moab/GeomUtil.hpp"
//#include "moab/FileOptions.hpp"
//#include "moab/GeomQueryTool.hpp"

McCAD::Conversion::BVHCreator::BVHCreator(const IO::InputConfig& inputConfig,
        const std::shared_ptr<TopTools_HSequenceOfShape> inputSolidsList){
    //: moabImpl{std::make_shared<moab::Core>()}{
    //loadFile(inputConfig.conversionFileName.c_str());
    //constructOBBTree();
    constructBVH(inputConfig.conversionFileName, inputSolidsList);
}

McCAD::Conversion::BVHCreator::~BVHCreator(){}

/*
void
McCAD::Conversion::BVHCreator::loadFile(const char* fileName){
    char options[120] = {0};
    moabImpl->create_meshset(moab::MESHSET_SET, fileSet);
    moabImpl->load_file(fileName, &fileSet, options);
}

void
McCAD::Conversion::BVHCreator::constructOBBTree(){
    GTT = std::make_shared<moab::GeomTopoTool> (moabImpl.get(), true);
    GTT->setup_implicit_complement();
    GTT->delete_all_obb_trees();
    std::cout << "vols: " << GTT->num_ents_of_dim(3) <<
                 "\nsurfs: " << GTT->num_ents_of_dim(2) << std::endl;
    GTT->construct_obb_trees();
}
*/
void
McCAD::Conversion::BVHCreator::constructBVH(const std::string& fileName,
                                            const std::shared_ptr<TopTools_HSequenceOfShape> inputSolidsList){
    BRep_Builder aBuilder;
    TopoDS_Compound aCompound;
    TopoDS_Shape aShape;
    aShape = inputSolidsList->Value(1);
    std::cout << "load file" << std::endl;
    BRepExtrema_ShapeList aFaceList;
    for (TopExp_Explorer i(aShape, TopAbs_FACE); i.More(); i.Next()){
        aFaceList.Append(TopoDS::Face(i.Current()));
    }
    std::cout << "face list" << std::endl;
    aBuilder.MakeCompound(aCompound);
    BRepMesh_IncrementalMesh aMesher(aShape, 1.0);
    std::cout << "mesh shape" << std::endl;
    BRepExtrema_TriangleSet aTriangleSet(aFaceList);
    std::cout << "triangle set, " << aTriangleSet.Size() << std::endl;
    const opencascade::handle<BVH_Tree<Standard_Real, 3> >& aBvh = aTriangleSet.BVH();
    std::cout << "construct bvh, " << aBvh->Length() <<std::endl;
    for (int i = 0; i < aBvh->Length(); i++){
        const BVH_Vec4i& aNodeData = aBvh->NodeInfoBuffer()[i];
        if (aNodeData.x() == 0){
            // inner node.
            const BVH_Vec3d& aP1 = aBvh->MinPoint(aNodeData.y());
            const BVH_Vec3d& aP2 = aBvh->MaxPoint(aNodeData.y());
            const BVH_Vec3d& aQ1 = aBvh->MinPoint(aNodeData.z());
            const BVH_Vec3d& aQ2 = aBvh->MaxPoint(aNodeData.z());
            try{
                BRepPrimAPI_MakeBox aBoxMaker(gp_Pnt(aP1.x(), aP1.y(), aP1.z()),
                                              gp_Pnt(aP2.x(), aP2.y(), aP2.z()));
                for (TopExp_Explorer i(aBoxMaker.Shape(), TopAbs_EDGE); i.More();
                     i.Next()){
                    aBuilder.Add(aCompound, i.Current());
                }
            } catch (Standard_Failure f){}
            try{
                BRepPrimAPI_MakeBox aBoxMaker(gp_Pnt(aQ1.x(), aQ1.y(), aQ1.z()),
                                              gp_Pnt(aQ2.x(), aQ2.y(), aQ2.z()));
                for (TopExp_Explorer i(aBoxMaker.Shape(), TopAbs_EDGE); i.More();
                     i.Next()){
                    aBuilder.Add(aCompound, i.Current());
                }
            } catch (Standard_Failure f){}
        } else{
            // leaves node.
            const BVH_Vec3d& aP1 = aBvh->MinPoint(i);
            const BVH_Vec3d& aP2 = aBvh->MaxPoint(i);
            try{
                BRepPrimAPI_MakeBox aBoxMaker(gp_Pnt(aP1.x(), aP1.y(), aP1.z()),
                                              gp_Pnt(aP2.x(), aP2.y(), aP2.z()));
                aBuilder.Add(aCompound, aBoxMaker.Shape());
            } catch (Standard_Failure f){}
        }
    }
    STEPControl_Writer writer1;
    writer1.Transfer(aCompound, STEPControl_StepModelType::STEPControl_AsIs);
    Standard_Integer kk = 0;
    std::string filename = "bvh.stp";
    writer1.Write(filename.c_str());
}
