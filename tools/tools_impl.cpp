// McCAD
#include "tools_impl.hpp"

const TopoDS_Shape
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape){
  TopoDS_Shape fixedSolidShape;
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

const TopoDS_Solid
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid){
  TopoDS_Solid fixedSolid;
  Handle_ShapeFix_Solid solidFix = new ShapeFix_Solid;
  solidFix->Init(solid);
  solidFix->Perform();
  fixedSolid = TopoDS::Solid(solidFix->Solid());
  return fixedSolid;
}

const TopoDS_Solid
McCAD::Tools::Preprocessor::Impl::genericFix(TopoDS_Solid& solid){
  TopoDS_Solid finalSolid;
  Handle_ShapeFix_Solid genericFix = new ShapeFix_Solid;
  genericFix->Init(solid);
  genericFix->Perform();
  finalSolid = TopoDS::Solid(genericFix->Solid());
  return finalSolid;
}

const Standard_Real
McCAD::Tools::Preprocessor::Impl::calcMeshDeflection(TopoDS_Solid& solid){
  Standard_Real deflection;
  /** Calculate the bounding box of face **/
  Standard_Real bndBoxGap = 0.0;
  Standard_Real converting = 100;
  Bnd_Box boundingBox;
  BRepBndLib::Add(solid,boundingBox);
  boundingBox.SetGap(bndBoxGap);
  Standard_Real Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
  boundingBox.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
  Standard_Real tempdeflection = std::max((Xmax-Xmin), (Ymax-Ymin));
  deflection = std::max(tempdeflection, (Zmax-Zmin)) / converting;
  return deflection;
}

const Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkBndSurfaces(TopoDS_Solid& solid){
  Standard_Boolean spline_torus_found = Standard_False;
  // Find the faces with small areas, they will not be added into face list.
  TopExp_Explorer explorer(solid, TopAbs_FACE);
  for (; explorer.More(); explorer.Next())
    {
      const TopoDS_Face& face = TopoDS::Face(explorer.Current());
      TopLoc_Location location;
      Handle_Geom_Surface geomSurface = BRep_Tool::Surface(face, location);
      GeomAdaptor_Surface surfAdaptor(geomSurface);

      if(surfAdaptor.GetType() == GeomAbs_Torus)
	{
	  std::cout << "    -- The current verion doesn't support processing of Tori. Ignoring solid!" << std::endl;
	  spline_torus_found = Standard_True;
	  break;
	}
      else if (surfAdaptor.GetType() == GeomAbs_BSplineSurface)
	{
	  std::cout << "    -- The current verion doesn't support processing of splines. Ignoring solid!" << std::endl;
	  spline_torus_found = Standard_True;
	  break;
	}
    }
  return spline_torus_found;
}
