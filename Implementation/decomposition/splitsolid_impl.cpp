// McCAD
#include "splitsolid_impl.hpp"

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::perform(const TopoDS_Solid& solid,
						const std::shared_ptr<Geometry::BoundSurface>& surface,
						std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList){
  calculateBoundingBox(solid);
  if (surface->getSurfaceType() == "Plane")
    {
      return split(solid, surface->accessSImpl()->extendedFace, subSolidsList);
    }
  else
    {
      return Standard_False;
    }
}

void
McCAD::Decomposition::SplitSolid::Impl::calculateBoundingBox(const TopoDS_Solid& solid){
  //std::cout << "calculateBoundingBox" << std::endl;
  BRepBndLib::Add(solid, bndBox);
  bndBox.SetGap(1.0);
  std::array<Standard_Real, 3> XYZmin;
  std::array<Standard_Real, 3> XYZmax;
  bndBox.Get(XYZmin[0], XYZmin[1], XYZmin[2], XYZmax[0], XYZmax[1], XYZmax[2]);
  boxSquareLength = sqrt(bndBox.SquareExtent());
  boundingBox = BRepPrimAPI_MakeBox(gp_Pnt(XYZmin[0], XYZmin[1], XYZmin[2]),
				    gp_Pnt(XYZmax[0], XYZmax[1], XYZmax[2])).Shape();
  //std::cout << "boxSquareLength: " << boxSquareLength << std::endl;
  //STEPControl_Writer writer0;
  //writer0.Transfer(boundingBox, STEPControl_StepModelType::STEPControl_AsIs);
  //writer0.Write("../examples/bbox.stp");
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
      //std::cout << "** Common fail" << std::endl;
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
	  //std::cout << "** checkRepair fail" << std::endl;
	  //throw std::runtime_error{"Shape problem"};
	  return Standard_False;
	}
    }
  else
    {
      //std::cout << "** splitWithBoxes fail" << std::endl;
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
	      //std::cout << "** cutter catch" << std::endl;
	      return Standard_False;
	    }
	}
    }
  catch(...)
    {
      //std::cout << "** common catch" << std::endl;
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
                Standard_Boolean rebuildCondition = rebuildSolidFromShell(tempSolid,
									  newSolid);
                if (!rebuildCondition)
		  {
		    //std::cout << "** rebuildSolidFromShell fail" << std::endl;
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

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::rebuildSolidFromShell(const TopoDS_Shape& solid,
							      TopoDS_Solid& resultSolid,
							      Standard_Real tolerance,
							      Standard_Real tolerance2){
  //std::cout << "rebuildSolidFromShell" << std::endl;
  BRepBuilderAPI_Sewing builder(tolerance, Standard_True, Standard_True, Standard_True,
				Standard_True);
  for (const auto& element : ShapeView<TopAbs_FACE>{solid})
    {
      TopoDS_Face tempFace = element;
      if (tempFace.IsNull())
	{
	  continue;
	}
      GProp_GProps geometryProperties;
      BRepGProp::SurfaceProperties(tempFace, geometryProperties);
      if (geometryProperties.Mass() <= tolerance2)
	{
	  continue;
	}
      builder.Add(tempFace);
    }

  builder.Perform();
  TopoDS_Shape tempShape = builder.SewedShape();

  TopoDS_Shape newshape;
  for (const auto& element : ShapeView<TopAbs_SHELL>{tempShape})
    {
      try
	{
	  TopoDS_Shell tempShell = element;
          ShapeFix_Solid tempSolid;
          tempSolid.LimitTolerance(tolerance);
          newshape = tempSolid.SolidFromShell(tempShell);
	}
      catch(...)
	{
	  return Standard_False;
	}
    }

  //std::cout << "analyzer" << std::endl;
  BRepCheck_Analyzer BRepAnalyzer(newshape, Standard_True);
  if (!(BRepAnalyzer.IsValid()))
    {
      return Standard_False;
    }
  else
    {
      resultSolid = TopoDS::Solid(newshape);
      return Standard_True;
    }
}
