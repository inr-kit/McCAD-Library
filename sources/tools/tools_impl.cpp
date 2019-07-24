// McCAD
#include "tools_impl.hpp"

void
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape, Standard_Real precision, Standard_Real maxTolerance){
  TopoDS_Shape fixedSolidShape;
  Handle_ShapeFix_FixSmallFace smallFaceFix = new ShapeFix_FixSmallFace;
  smallFaceFix->Init(solidShape);
  smallFaceFix->SetPrecision(precision);
  smallFaceFix->SetMaxTolerance(maxTolerance);
  smallFaceFix->Perform();
  solidShape = smallFaceFix->FixShape();
}

void
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid){
  TopoDS_Solid fixedSolid;
  Handle_ShapeFix_Solid solidFix = new ShapeFix_Solid;
  solidFix->Init(solid);
  solidFix->Perform();
  solid = TopoDS::Solid(solidFix->Solid());
}

void
McCAD::Tools::Preprocessor::Impl::genericFix(TopoDS_Solid& solid){
  TopoDS_Solid finalSolid;
  Handle_ShapeFix_Solid genericFix = new ShapeFix_Solid;
  genericFix->Init(solid);
  genericFix->Perform();
  solid = TopoDS::Solid(genericFix->Solid());
}

Standard_Real
McCAD::Tools::Preprocessor::Impl::calcMeshDeflection(const TopoDS_Solid& solid, Standard_Real bndBoxGap, Standard_Real converting){
  Standard_Real deflection;
  /** Calculate the bounding box of face **/
  Bnd_Box boundingBox;
  BRepBndLib::Add(solid,boundingBox);
  boundingBox.SetGap(bndBoxGap);
  Standard_Real Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
  boundingBox.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
  Standard_Real tempdeflection = std::max((Xmax-Xmin), (Ymax-Ymin));
  deflection = std::max(tempdeflection, (Zmax-Zmin)) / converting;
  return deflection;
}

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkBndSurfaces(const TopoDS_Solid& solid){
  Standard_Boolean spline_torus_found = Standard_False;
  // Find the faces with small areas, they will not be added into face list.
  TopExp_Explorer explorer(solid, TopAbs_FACE);
  for (; explorer.More(); explorer.Next())
    {
      TopoDS_Face face = TopoDS::Face(explorer.Current());
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

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkFace(const TopoDS_Face& face, Standard_Real tolerance){
  ShapeAnalysis_CheckSmallFace shapeAnalysis;
  Standard_Boolean isSmallFace = Standard_False;
  TopoDS_Edge edge1, edge2;
  if( shapeAnalysis.CheckSpotFace(face, tolerance) || shapeAnalysis.CheckStripFace(face, edge1, edge2, tolerance))
    {
      Standard_Boolean isSmallFace = Standard_True;
    }
  return isSmallFace;
}

void
McCAD::Tools::Preprocessor::Impl::fixFace(TopoDS_Face& face, Standard_Real precision, Standard_Real maxTolerance){
  Handle_ShapeFix_Shape shapeFixer = new ShapeFix_Shape(face);
  shapeFixer->SetPrecision(precision);
  shapeFixer->SetMaxTolerance(maxTolerance);
  shapeFixer->FixWireTool()->FixRemovePCurveMode() = 1;
  shapeFixer->FixWireTool()->FixConnected();
  shapeFixer->Perform();
  face = TopoDS::Face(shapeFixer->Shape());
}

std::string
McCAD::Tools::Preprocessor::Impl::getSurfTypeName(const Standard_Integer& index){
  // This function is to be deleted and has to write a tool to get the type from the enumerated type from OCE rather than copy the types into a vector as is done here!.
  std::vector<std::string> typeNames = {
    "Plane",
    "Cylinder",
    "Cone",
    "Sphere",
    "Torus",
    "BezierSurface",
    "BSplineSurface",
    "SurfaceOfRevolution",
    "SurfaceOfExtrusion",
    "OffsetSurface",
    "OtherSurface"
  };
  return typeNames[index];
}

std::string
McCAD::Tools::Preprocessor::Impl::getCurveTypeName(const Standard_Integer& index){
  // This function is to be deleted. A tool is to be written to get the type from the enumerated type from OCE rather than copy the types into a vector as is done here!.
  std::vector<std::string> typeNames = {
    "Line",
    "Circle",
    "Ellipse",
    "Hyperbola",
    "Parabola",
    "BezierCurve",
    "BSplineCurve",
    "OtherCurve"
  };
  return typeNames[index];
}

gp_Dir
McCAD::Tools::Preprocessor::Impl::normalOnFace(const TopoDS_Face& aFace, const gp_Pnt& aPoint){
  Standard_Real uParameter, vParameter;
  BRepAdaptor_Surface surfaceAdaptor(aFace, Standard_True);

  switch (surfaceAdaptor.GetType())
    {
    case GeomAbs_Plane:
      ElSLib::Parameters(surfaceAdaptor.Plane(), aPoint, uParameter, vParameter);
      break;

    case GeomAbs_Cylinder:
      ElSLib::Parameters(surfaceAdaptor.Cylinder(), aPoint, uParameter, vParameter);
      break;

    case GeomAbs_Cone:
      ElSLib::Parameters(surfaceAdaptor.Cone(), aPoint, uParameter, vParameter);
      break;

    case GeomAbs_Torus:
      ElSLib::Parameters(surfaceAdaptor.Torus(), aPoint, uParameter, vParameter);
      break;

    default:
      {
	return gp_Dir(1., 0., 0.);
      }
    }

  gp_Pnt point;
  gp_Vec D1U, D1V;
  surfaceAdaptor.D1(uParameter, vParameter, point, D1U, D1V);
  gp_Dir direction;
  CSLib_DerivativeStatus derivativeStatus;
  CSLib::Normal(D1U, D1V, Precision::Confusion(), derivativeStatus, direction);
  if (aFace.Orientation() == TopAbs_FORWARD)
    direction.Reverse();
  return direction;
}

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::isSameSurface(const TopoDS_Face& firstFace, const TopoDS_Face& secondFace){
  // Check surface type.
  TopLoc_Location location;
  Handle_Geom_Surface firstSurface = BRep_Tool::Surface(firstFace, location);
  GeomAdaptor_Surface firstSurfaceAdaptor(firstSurface);
  Handle_Geom_Surface secondSurface = BRep_Tool::Surface(secondFace, location);
  GeomAdaptor_Surface secondSurfaceAdaptor(secondSurface);
  if (firstSurfaceAdaptor.GetType() != secondSurfaceAdaptor.GetType())
    {
      return Standard_False;
    }
  // Check orientation.
  TopAbs_Orientation firstOrientation = firstFace.Orientation();
  TopAbs_Orientation secondOrientation = secondFace.Orientation();
  if (firstOrientation != secondOrientation)
    {
      return Standard_False;
    }
  // Check direction and parameters.
  std::vector<Standard_Real> firstPlaneParameters(4);
  gp_Pln firstPlane = firstSurfaceAdaptor.Plane();
  firstPlane.Scale(gp_Pnt(0.0, 0.0, 0.0), GetUnit());
  firstPlaneAxis = firstPlane.Position();
  firstPlane.Coefficients(firstPlaneParameters[0], firstPlaneParameters[1], firstPlaneParameters[2], firstPlaneParameters[3]);
  for (Standard_Integer i = 0, i <= firstPlaneParameters.size(), ++i)
    {
      if (firstPlaneParameters[i] <= parameterTolerance)
	{
	  *firstPlaneParameters[i] = 0.0;
	}
    }
  firstPlaneDirection.SetCoord(firstPlaneParameters[0], firstPlaneParameters[1], firstPlaneParameters[2]);
  firstPlanePoint = firstPlaneAxis.Location();

  std::vector<Standard_Real> secondPlaneParameters(4);
  gp_Pln secondPlane = secondSurfaceAdaptor.Plane();
  secondPlane.Scale(gp_Pnt(0.0, 0.0, 0.0), GetUnit());
  secondPlaneAxis = secondPlane.Position();
  secondPlane.Coefficients(secondPlaneParameters[0], secondPlaneParameters[1], secondPlaneParameters[2], secondPlaneParameters[3]);
  for (Standard_Integer i = 0, i <= secondPlaneParameters.size(), ++i)
    {
      if (secondPlaneParameters[i] <= parameterTolerance)
        {
          *secondPlaneParameters[i] = 0.0;
        }
    }
  secondPlaneDirection.SetCoord(secondPlaneParameters[0], secondPlaneParameters[1], secondPlaneParameters[2]);
  secondPlanePoint = secondPlaneAxis.Location();

  if (firstPlaneDirection.IsEqual(secondPlaneDirection, angleTolerance) && std::Abs(firstPlaneParameters[3] - secondPlaneParameters[3]) <= distanceTolerance)
    {
    }
  else if (firstPlaneDirection.IsOpposite(secondPlaneDirection, angleTolerance) && !std::Abs(firstPlaneParameters[3] - secondPlaneParameters[3]) <= distanceTolerance)
    {
    }
  else
    {
      return Standard_False;
    }
  return Standard_True;
}
