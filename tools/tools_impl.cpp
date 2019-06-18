// McCAD
#include "tools_impl.hpp"

//const Handle_TopTools_HSequenceOfShape&
void McCAD::Tools::Preprocessor::Impl::splitCompSolids(Handle_TopTools_HSequenceOfShape& solidsList)
{
  // Used to tranform the pointer from one on the heap to a normal pointer
  //Handle_TopTools_HSequenceOfShape splitCompSolidsList;
  this->splitCompSolidsList = new TopTools_HSequenceOfShape;
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
  //return splitCompSolidsList;
}

const TopoDS_Shape&
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape)
{
  Handle(ShapeFix_FixSmallFace) smallFaceFix = new ShapeFix_FixSmallFace();
  smallFaceFix->Init(solidShape);
  smallFaceFix->SetPrecision(0.001);
  smallFaceFix->SetMaxTolerance(0.001);
  smallFaceFix->Perform();
  this->fixedSolidShape = smallFaceFix->FixShape();
  //theShape.Free();
  return fixedSolidShape;
}

const TopoDS_Solid&
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid)
{
  Handle(ShapeFix_Solid) solidFix = new ShapeFix_Solid;
  solidFix->Init(solid);
  solidFix->Perform();
  //theSolid.Free();
  this->fixedSolid = TopoDS::Solid(solidFix->Solid());
  return fixedSolid;
}
