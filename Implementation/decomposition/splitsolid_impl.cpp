// McCAD
#include "splitsolid_impl.hpp"
#include "SolidRebuilder.hpp"
#include "SolidSplitter.hpp"

// C++
#include <iterator>

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::perform(
        const TopoDS_Solid& solid,
        const std::shared_ptr<Geometry::BoundSurface>& surface,
        std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList){
    calculateBoundingBox(solid);
    if (surface->getSurfaceType() == "Plane"){
        return split(solid, surface->accessSImpl()->extendedFace, subSolidsList);
    }else{
        return Standard_False;
    }
}

void
McCAD::Decomposition::SplitSolid::Impl::calculateBoundingBox(
        const TopoDS_Solid& solid){
  std::cout << "calculateBoundingBox" << std::endl;
  BRepBndLib::Add(solid, bndBox);
  bndBox.SetGap(0.5);
  std::array<Standard_Real, 3> XYZmin;
  std::array<Standard_Real, 3> XYZmax;
  bndBox.Get(XYZmin[0], XYZmin[1], XYZmin[2], XYZmax[0], XYZmax[1], XYZmax[2]);
  boxSquareLength = sqrt(bndBox.SquareExtent());
  std::cout << "boxSquareLength: " << boxSquareLength << std::endl;
  std::array<Standard_Real, 3> center;
  std::array<Standard_Real, 3> dimension;
  gp_Pnt corners[8];
  bndBox.GetVertex(corners);
  std::cout << "corners: " << std::endl;
  for (Standard_Integer k = 0; k < 8; ++k)
    {
      std::cout << corners[k].X() << ", " << corners[k].Y() << ", " << corners[k].Z() << std::endl;
    }
  center = {bndBox.Center().X(), bndBox.Center().Y(), bndBox.Center().Z()};
  std::cout << "center: " << bndBox.Center().X() << ", " << bndBox.Center().Y() << ", " << bndBox.Center().Z() << std::endl;
  dimension = {bndBox.XHSize(), bndBox.YHSize(), bndBox.ZHSize()};
  std::cout << "xdirection: " << bndBox.XDirection().X() << ", " << bndBox.XDirection().Y()  << ", " << bndBox.XDirection().Z() << std::endl;
  std::cout << "ydirection: " << bndBox.YDirection().X() << ", " << bndBox.YDirection().Y() << ", " << bndBox.YDirection().Z() << std::endl;
  std::cout << "zdirection: " << bndBox.ZDirection().X() << ", " << bndBox.ZDirection().Y() << ", " << bndBox.ZDirection().Z() << std::endl;
  std::cout << "dimentsions: " << bndBox.XHSize() << ", " << bndBox.YHSize()  << ", " << bndBox.ZHSize() << std::endl;
  gp_Pnt pnt1 = corners[0];
  gp_Pnt pnt2 = corners[7];

  TopLoc_Location location = solid.Location();
  gp_Trsf transformation_solid;
  transformation_solid = location.Transformation();
  for (Standard_Integer i = 1; i < 4; ++i)
    {
      std::cout << transformation_solid.Value(i,1) << ", " << transformation_solid.Value(i,2) << ", " << transformation_solid.Value(i,3) << ", " << transformation_solid.Value(i,4) << std::endl;
    }

  std::cout << "Axis" << std::endl;
  gp_Ax3 axis(gp_Pnt(0, 0, 0), bndBox.ZDirection(), bndBox.XDirection());
  //axis.SetDirection(gp_Dir(bndBox.ZDirection()));
  //axis.SetXDirection(gp_Dir(bndBox.XDirection()));
  //axis.SetYDirection(gp_Dir(bndBox.YDirection()));
  std::cout << "xdirection: " << axis.XDirection().X() << ", " << axis.XDirection().Y()  << ", " << axis.XDirection().Z() << std::endl;
  std::cout << "ydirection: " << axis.YDirection().X() << ", " << axis.YDirection().Y() << ", " << axis.YDirection().Z() << std::endl;
  std::cout << "zdirection: " << axis.Direction().X() << ", " << axis.Direction().Y() << ", " << axis.Direction().Z() << std::endl;

  std::cout << "Transformation" << std::endl;
  gp_Trsf transformation;
  transformation.SetTransformation(axis);
  for (Standard_Integer i = 1; i < 4; ++i)
    {
      std::cout << transformation.Value(i,1) << ", " << transformation.Value(i,2) << ", " << transformation.Value(i,3) << ", " << transformation.Value(i,4) << std::endl;
    }
  
  //std::cout << "try Pnt1" << std::endl;
  pnt1 = pnt1.Transformed(transformation.Inverted());
  //std::cout << "try Pnt2" << std::endl;
  pnt2 = pnt2.Transformed(transformation.Inverted());
  std::cout << "Pnt1: " << pnt1.X() << ", " << pnt1.Y()  << ", " << pnt1.Z() << std::endl;
  std::cout << "Pnt2: " << pnt2.X() << ", " << pnt2.Y()  << ", " << pnt2.Z() << std::endl;

  std::cout << "BRepPrimAPI_MakeBox" << std::endl;
  boundingBox = BRepPrimAPI_MakeBox(pnt1, pnt2).Solid();
  location = boundingBox.Location();
  transformation_solid = location.Transformation();
  for (Standard_Integer i = 1; i < 4; ++i)
    {
      std::cout << transformation_solid.Value(i,1) << ", " << transformation_solid.Value(i,2) << ", " << transformation_solid.Value(i,3) << ", " << transformation_solid.Value(i,4) << std::endl;
    }

  std::cout << "Transform OBB solid" << std::endl;
  //transformation = transformation.Inverted();
  BRepBuilderAPI_Transform boxTransform(transformation);
  boxTransform.Perform(boundingBox);
  boundingBox = boxTransform.ModifiedShape(boundingBox);
  
  STEPControl_Writer writer0;
  writer0.Transfer(boundingBox, STEPControl_StepModelType::STEPControl_AsIs);
  writer0.Transfer(solid, STEPControl_StepModelType::STEPControl_AsIs);
  Standard_Integer kk = 0;
  std::string filename = "../examples/bbox/box";
  std::string suffix = ".stp";
  while (std::filesystem::exists(filename + std::to_string(kk) + suffix))
    {
      ++kk;
    }
  filename += std::to_string(kk);
  filename += suffix;
  writer0.Write(filename.c_str());
}

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::split(
        const TopoDS_Solid& solid,
        const TopoDS_Face& splitFace,
        std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList){

    auto halfSolids = SolidSplitter{}(solid, splitFace);
    if(!halfSolids) return false;

    subSolidsList->Append(halfSolids->first);
    subSolidsList->Append(halfSolids->second);
    return checkRepair(subSolidsList);
}


Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::checkRepair(
        std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList,
        Standard_Real tolerance){

    std::unique_ptr<TopTools_HSequenceOfShape> newsubSolidsList
            = std::make_unique<TopTools_HSequenceOfShape>();

    for(Standard_Integer i = 1; i <= subSolidsList->Length(); ++i){
        for(const auto& tmpsolid : ShapeView<TopAbs_SOLID>{subSolidsList->Value(i)}){
            if(tmpsolid.IsNull()) continue;
            GProp_GProps geometryProperties;
            BRepGProp::VolumeProperties(subSolidsList->Value(i), geometryProperties);

            if(geometryProperties.Mass() <= tolerance) continue;
            newsubSolidsList->Append(tmpsolid);
        }
    }

    bool res = repair(*newsubSolidsList);
    if(res)
        subSolidsList = std::move(newsubSolidsList);

    return res;
}

bool
McCAD::Decomposition::SplitSolid::Impl::repair(
        TopTools_HSequenceOfShape& listOfSolids) const{

    for(Standard_Integer i = 1; i <= listOfSolids.Length(); ++i){
        const auto& tempSolid = TopoDS::Solid(listOfSolids.Value(i));
        BRepCheck_Analyzer BRepAnalyzer(tempSolid, Standard_True);
        if(BRepAnalyzer.IsValid()) continue;

        auto rebuiltSolid = SolidRebuilder{}(tempSolid);
        if(!rebuiltSolid){
            return false;
        }else{
            listOfSolids.InsertAfter(i, *rebuiltSolid);
            listOfSolids.Remove(i);
        }
    }
    return true;
}




