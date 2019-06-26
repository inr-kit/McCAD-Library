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

const Standard_Real&
McCAD::Tools::Preprocessor::Impl::calMeshDeflection(TopoDS_Solid& solid){
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

const Standard_Boolean&
McCAD::Tools::Preprocessor::Impl::checkBndSurfaces(TopoDS_Solid& solid){
  TopExp_Explorer exF;

  // Find the faces with small areas, they will not be added into face list.
  for (exF.Init(solid, TopAbs_FACE); exF.More(); exF.Next())
    {
      TopoDS_Face face = TopoDS::Face(exF.Current());
      TopLoc_Location loc;
      Handle_Geom_Surface geomSurface = BRep_Tool::Surface(face, loc);
      GeomAdaptor_Surface surfAdoptor(geomSurface);

      if(surfAdoptor.GetType() == GeomAbs_Torus)
	{
	  std::cout << "torus" << std::endl;
	  std::cout << "    - Solid containes a torus. Ignoring solid!." << std::endl;
	  goto terminatefor;
	}
      else if (surfAdoptor.GetType() == GeomAbs_BSplineSurface)
	{
	  std::cout << "spline" << std::endl;
	  std::cout << "    - Solid containes a spline. Ignoring solid!." << std::endl;
	  goto terminatefor;
	}
    }
  return Standard_False;
 terminatefor:
  return Standard_True;
}


