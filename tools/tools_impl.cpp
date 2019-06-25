// McCAD
#include "tools_impl.hpp"

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
