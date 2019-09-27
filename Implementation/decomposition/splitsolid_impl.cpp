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
  //std::cout << "boxSquareLength: " << boxSquareLength << std::endl;
  boundingBox = BRepPrimAPI_MakeBox(gp_Pnt(XYZmin[0], XYZmin[1], XYZmin[2]),
				    gp_Pnt(XYZmax[0], XYZmax[1], XYZmax[2])).Shape();

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




