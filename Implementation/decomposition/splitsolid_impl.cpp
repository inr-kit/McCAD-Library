// McCAD
#include "splitsolid_impl.hpp"
#include "SolidRebuilder.hpp"

// C++
#include <iterator>

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::perform(Geometry::Solid::Impl& solidImpl,
						Standard_Integer indexSplitSurface){
  auto surface = solidImpl.selectedSplitFacesList[indexSplitSurface];

  createOBBSolid(solidImpl.OBB, solidImpl.solid);
  if (surface->getSurfaceType() == "Plane")
    {
      return split(solidImpl.solid, surface->accessSImpl()->extendedFace,
		   solidImpl.splitSolidList);
    }
  else
    {
      std::cout << "** not Plane" << std::endl; 
      return Standard_False;
    }
}

void
McCAD::Decomposition::SplitSolid::Impl::createOBBSolid(const Bnd_OBB& OBB,
						       TopoDS_Solid& solid){
  std::cout << "createOBBSolid" << std::endl;
  bndBox = OBB;
  bndBox.Enlarge(0.4);
  std::cout << "status: " << bndBox.IsAABox() << std::endl;
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
McCAD::Decomposition::SplitSolid::Impl::split(const TopoDS_Solid& solid,
					      const TopoDS_Face& splitFace,
					      std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList){
  //std::cout << "split" << std::endl;
  gp_Pnt positivePoint, negativePoint;
  calculatePoints(splitFace, positivePoint, negativePoint);
  //std::cout << "positive point: " << positivePoint.X() << "," << positivePoint.Y() << "," << positivePoint.Z() << "," << std::endl;
  //std::cout << "negative point: " << negativePoint.X() << "," << negativePoint.Y() << "," << negativePoint.Z() << "," << std::endl;
  
  TopoDS_Solid positiveHalfSolid = BRepPrimAPI_MakeHalfSpace(splitFace,
							     positivePoint).Solid();
  TopoDS_Solid negativeHalfSolid = BRepPrimAPI_MakeHalfSpace(splitFace,
							     negativePoint).Solid();
  TopoDS_Shape positiveBox, negativeBox;
  try
    {
      BRepAlgoAPI_Common positiveCommon(positiveHalfSolid, boundingBox);
      if (positiveCommon.IsDone())
	{
	  //std::cout << "positiveCommon" << std::endl;
	  positiveBox = positiveCommon.Shape();

	  //STEPControl_Writer writer1;
	  //writer1.Transfer(positiveBox, STEPControl_StepModelType::STEPControl_AsIs);
	  //writer1.Transfer(positiveHalfSolid, STEPControl_StepModelType::STEPControl_AsIs);
	  //writer1.Write("../examples/positive.stp");
	}
      BRepAlgoAPI_Common negativeCommon(negativeHalfSolid, boundingBox);
      if (negativeCommon.IsDone())
	{
	  //std::cout << "negativeCommon" << std::endl;
	  negativeBox = negativeCommon.Shape();

	  //STEPControl_Writer writer2;
	  //writer2.Transfer(negativeBox, STEPControl_StepModelType::STEPControl_AsIs);
          //writer2.Transfer(negativeHalfSolid, STEPControl_StepModelType::STEPControl_AsIs);
	  //writer2.Write("../examples/negative.stp");
	}
    }
  catch(...)
    {
      std::cout << "** Common fail" << std::endl;
      return Standard_False;
    }

  if (splitWithBoxes(solid, positiveBox, negativeBox, subSolidsList) &&
      splitWithBoxes(solid, negativeBox, positiveBox, subSolidsList))
    {
      if (checkRepair(subSolidsList))
	{
	  return Standard_True;
	}
      else
	{
	  std::cout << "** checkRepair fail" << std::endl;
	  //throw std::runtime_error{"Shape problem"};
	  return Standard_False;
	}
    }
  else
    {
      std::cout << "** splitWithBoxes fail" << std::endl;
      return Standard_False;
    }
}

void
McCAD::Decomposition::SplitSolid::Impl::calculatePoints(const TopoDS_Face& splitFace,
							gp_Pnt& positivePoint,
							gp_Pnt& negativePoint){
  //std::cout << "calculatePoints" << std::endl;
  BRepAdaptor_Surface splitSurface(splitFace, Standard_True);
  GeomAdaptor_Surface surfaceAdaptor = splitSurface.Surface();
  if (surfaceAdaptor.GetType() == GeomAbs_Plane)
    {
      gp_Pln plane = surfaceAdaptor.Plane();
      gp_Ax3 position = plane.Position();
      gp_Dir direction = position.Direction();
      gp_Pnt location = position.Location();

      //std::cout << "location point: " << location.X() << "," << location.Y() << "," << location.Z() << "," << std::endl;

      positivePoint.SetX(location.X() + 100*direction.X());
      positivePoint.SetY(location.Y() + 100*direction.Y());
      positivePoint.SetZ(location.Z() + 100*direction.Z());

      negativePoint.SetX(location.X() - 100*direction.X());
      negativePoint.SetY(location.Y() - 100*direction.Y());
      negativePoint.SetZ(location.Z() - 100*direction.Z());
    }
}

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::splitWithBoxes(const TopoDS_Solid& solid,
						       TopoDS_Shape& firstBox,
						       TopoDS_Shape& secondBox,
						       std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList){
  //std::cout << "splitWithBoxes" << std::endl;
  // Try first with common boolean operations and if failed use subtract
  try
    {
      BRepAlgoAPI_Common common(firstBox, solid);
      if (common.IsDone())
	{
	  TopoDS_Shape subSolid = common.Shape();
	  subSolidsList->Append(subSolid);
	  //std::cout << "common" << std::endl;
	  return Standard_True;
	}
      else
	{
	  try
	    {
	      BRepAlgoAPI_Cut cutter(solid, secondBox);
	      if (cutter.IsDone())
		{
		  TopoDS_Shape subSolid = cutter.Shape();
		  subSolidsList->Append(subSolid);
		  //std::cout << "cutter" << std::endl;
		  return Standard_True;
		}
	    }
	  catch(...)
	    {
	      std::cout << "** cutter catch" << std::endl;
	      return Standard_False;
	    }
	}
    }
  catch(...)
    {
      std::cout << "** common catch" << std::endl;
      return Standard_False;
    }
  return Standard_False;
}

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::checkRepair(std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList,
						    Standard_Real tolerance){
  //std::cout << "checkRepair" << std::endl;
    std::unique_ptr<TopTools_HSequenceOfShape> newsubSolidsList = std::make_unique<TopTools_HSequenceOfShape>();
    for(Standard_Integer i = 1; i <= subSolidsList->Length(); ++i)
      {
        for(const auto& tmpsolid : ShapeView<TopAbs_SOLID>{subSolidsList->Value(i)})
	  {
	    //BRepTools::Write(subSolidsList->Value(i), "../examples/dump.brep");
	    if (tmpsolid.IsNull())
	      {
		//std::cout << "** null solid" << std::endl;
                continue;
	      }
            GProp_GProps geometryProperties;
            BRepGProp::VolumeProperties(subSolidsList->Value(i), geometryProperties);

            if(geometryProperties.Mass() <= tolerance)
	      {
		//std::cout << "** volume < tol" << std::endl;
                continue;
	      }
	    //STEPControl_Writer writer;
	    //writer.Transfer(tmpsolid, STEPControl_StepModelType::STEPControl_AsIs);
	    //writer.Write("../examples/tmp.stp");
            newsubSolidsList->Append(tmpsolid);
	  }
      }
    //std::cout << "new list: " << newsubSolidsList->Length() << std::endl;

    if(newsubSolidsList->Length() > 0)
      {
        for (Standard_Integer i = 1; i <= newsubSolidsList->Length(); ++i)
	  {
            //std::cout << newsubSolidsList->Value(i).IsNull() << std::endl;
            TopoDS_Solid tempSolid = TopoDS::Solid(newsubSolidsList->Value(i));
            //std::cout << "tempSolid" << std::endl;
            BRepCheck_Analyzer BRepAnalyzer(tempSolid, Standard_True);
            //std::cout << "BRepAnalyzer" << std::endl;
            if(!(BRepAnalyzer.IsValid()))
	      {
                //std::cout << "build solid" << std::endl;
                TopoDS_Solid newSolid;
                auto rebuiltSolid = SolidRebuilder{}(tempSolid);
                if (!rebuiltSolid)
		  {
		    std::cout << "** rebuildSolidFromShell fail" << std::endl;
                    return Standard_False;
		  }
		else
		  {
                    newsubSolidsList->InsertAfter(i, newSolid);
                    newsubSolidsList->Remove(i);
		  }
	      }
	    else
	      {
		//std::cout << "solid is valid" << std::endl;
                continue;
	      }
	  }
	subSolidsList = std::move(newsubSolidsList);
      }
    return Standard_True;
}


