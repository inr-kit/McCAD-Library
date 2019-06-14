// McCAD
#include "tools_impl.hpp"

const Handle_TopTools_HSequenceOfShape
SplitCompSolids(Handle_TopTools_HSequenceOfShape& solidsList)
{
  // Used to tranform the pointer from one on the heap to a normal pointer
  Handle_TopTools_HSequenceOfShape splitCompSolidsList;
  splitCompSolidsList = new TopTools_HSequenceOfShape;
  for (int i=1; i <= solidsList->Length(); i++)
    {
    if ((solidsList->Value(i)).ShapeType() == TopAbs_COMPSOLID ||
	(solidsList->Value(i)).ShapeType() == TopAbs_COMPOUND)
      {
      TopExp_Explorer ex;
      for (ex.Init(solidsList->Value(i), TopAbs_SOLID); ex.More(); ex.Next())
	{
	TopoDS_Solid tmpSol = TopoDS::Solid(ex.Current());
	splitCompSolidsList->Append(tmpSol);
	}
      }
    else if ((solidsList->Value(i)).ShapeType() == TopAbs_SOLID)
      {
      splitCompSolidsList->Append(solidsList->Value(i));
      }
    }
  return splitCompSolidsList;
}

const TopoDS_Shape& removeSmallFaces(TopoDS_Shape& solidShape) const
{
  Handle(ShapeFix_FixSmallFace) smallFaceFix = new ShapeFix_FixSmallFace();
  smallFaceFix->Init(solidShape);
  smallFaceFix->SetPrecision(0.001);
  smallFaceFix->SetMaxTolerance(0.001);
  smallFaceFix->Perform();
  TopoDS_Shape fixedSolidShape = smallFaceFix->FixShape();
  //theShape.Free();
  return fixedSolidShape;
}

const TopoDS_Solid& repairSolid(TopoDS_Solid& solid) const
{
  Handle(ShapeFix_Solid) solidFix = new ShapeFix_Solid;
  solidFix->Init(solid);
  solidFix->Perform();
  //theSolid.Free();
  TopoDS_Solid fixedSolid = TopoDS::Solid(solidFix->Solid());
  return fixedSolid;
}
