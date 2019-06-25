// McCAD
#include "tools_impl.hpp"

const TopoDS_Shape&
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape){
  Standard_Real precision = 0.001;
  Standard_Real maxTolerance = 0.001;
  Handle_ShapeFix_FixSmallFace smallFaceFix = new ShapeFix_FixSmallFace;
  smallFaceFix->Init(solidShape);
  smallFaceFix->SetPrecision(precision);
  smallFaceFix->SetMaxTolerance(maxTolerance);
  smallFaceFix->Perform();
  fixedSolidShape = smallFaceFix->FixShape();
  return fixedSolidShape;
}

const TopoDS_Solid&
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid){
  Handle_ShapeFix_Solid solidFix = new ShapeFix_Solid;
  solidFix->Init(solid);
  solidFix->Perform();
  fixedSolid = TopoDS::Solid(solidFix->Solid());
  return fixedSolid;
}

const TopoDS_Solid&
McCAD::Tools::Preprocessor::Impl::genericFix(TopoDS_Solid& solid){
  Handle_ShapeFix_Solid genericFix = new ShapeFix_Solid;
  genericFix->Init(solid);
  genericFix->Perform();
  finalSolid = TopoDS::Solid(genericFix->Solid());
  return finalSolid;
}

