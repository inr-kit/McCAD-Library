// McCAD
#include "tools_impl.hpp"
#include <vector>
#include <array>

void
McCAD::Tools::Preprocessor::Impl::checkBndSurfaces(const TopoDS_Solid& solid, Standard_Boolean& isTorus, Standard_Boolean& isSpline){
  TopoDS_Face face;
  TopExp_Explorer explorer(solid, TopAbs_FACE);
  for (; explorer.More(); explorer.Next())
    {
      face = TopoDS::Face(explorer.Current());
      GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));

      if(surfAdaptor.GetType() == GeomAbs_Torus)
        {
          std::cout << "    -- The current verion doesn't support processing of Tori. Ignoring solid!" << std::endl;
          isTorus = Standard_True;
        }
      else if (surfAdaptor.GetType() == GeomAbs_BSplineSurface)
        {
          std::cout << "    -- The current verion doesn't support processing of splines. Ignoring solid!" << std::endl;
          isSpline = Standard_True;
        }
    }
}

void
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape, Standard_Real precision, Standard_Real maxTolerance){
  // If the size of face is less than the precision (spot or strip face), it will be removed.
  Handle_ShapeFix_FixSmallFace smallFaceFix = new ShapeFix_FixSmallFace;
  smallFaceFix->Init(solidShape);
  smallFaceFix->SetPrecision(precision);
  smallFaceFix->SetMaxTolerance(maxTolerance);
  smallFaceFix->Perform();
  solidShape = smallFaceFix->FixShape();
}

void
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid){
  Handle_ShapeFix_Solid solidFix = new ShapeFix_Solid;
  solidFix->Init(solid);
  solidFix->Perform();
  solid = TopoDS::Solid(solidFix->Solid());
}

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkFace(const TopoDS_Face& face, Standard_Real tolerance){
  ShapeAnalysis_CheckSmallFace shapeAnalysis;
  TopoDS_Edge edge1, edge2;
  if( shapeAnalysis.CheckSpotFace(face, tolerance) || shapeAnalysis.CheckStripFace(face, edge1, edge2, tolerance))
    {
      return Standard_True;
    }
  return Standard_False;
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
  gp_Dir normal;
  CSLib_DerivativeStatus derivativeStatus;
  CSLib::Normal(D1U, D1V, Precision::Confusion(), derivativeStatus, normal);
  if (aFace.Orientation() == TopAbs_FORWARD)
    normal.Reverse();
  return normal;
}

<<<<<<< HEAD
Standard_Boolean
McCAD::Tools::Preprocessor::Impl::isSamePlane(const TopoDS_Face& firstFace, const TopoDS_Face& secondFace, Standard_Real distanceTolerance, Standard_Real angleTolerance, Standard_Real parameterTolerance){
  // Check orientation.
  if (firstFace.Orientation() != secondFace.Orientation())
    {
      return Standard_False;
    }

  // Check surface type.
  Handle_Geom_Surface firstSurface = BRep_Tool::Surface(firstFace);
  GeomAdaptor_Surface firstSurfaceAdaptor(firstSurface);
  Handle_Geom_Surface secondSurface = BRep_Tool::Surface(secondFace);
  GeomAdaptor_Surface secondSurfaceAdaptor(secondSurface);
  if (firstSurfaceAdaptor.GetType() != secondSurfaceAdaptor.GetType())
    {
      return Standard_False;
    }

  try
    {
      // Check direction and parameters.
      std::array<Standard_Real, 4> firstPlaneParameters;
      gp_Pln firstPlane = firstSurfaceAdaptor.Plane();
      firstPlane.Scale(gp_Pnt(0.0, 0.0, 0.0), 0.1);
      gp_Ax3 firstPlaneAxes = firstPlane.Position();
      firstPlane.Coefficients(firstPlaneParameters[0], firstPlaneParameters[1], firstPlaneParameters[2], firstPlaneParameters[3]);
      for (Standard_Integer i = 0; i <= firstPlaneParameters.size() - 1; ++i)
	{
          if (firstPlaneParameters[i] <= parameterTolerance)
	    {
	      firstPlaneParameters[i] = 0.0;
	    }
        }
      gp_Dir firstPlaneDirection;
      firstPlaneDirection.SetCoord(firstPlaneParameters[0], firstPlaneParameters[1], firstPlaneParameters[2]);
      gp_Pnt firstPlanePoint = firstPlaneAxes.Location();

      std::array<Standard_Real, 4> secondPlaneParameters;
      gp_Pln secondPlane = secondSurfaceAdaptor.Plane();
      secondPlane.Scale(gp_Pnt(0.0, 0.0, 0.0), 0.1);
      gp_Ax3 secondPlaneAxes = secondPlane.Position();
      secondPlane.Coefficients(secondPlaneParameters[0], secondPlaneParameters[1], secondPlaneParameters[2], secondPlaneParameters[3]);
      for (Standard_Integer i = 0; i <= secondPlaneParameters.size() - 1; ++i)
        {
          if (secondPlaneParameters[i] <= parameterTolerance)
            {
              secondPlaneParameters[i] = 0.0;
            }
        }
      gp_Dir secondPlaneDirection;
      secondPlaneDirection.SetCoord(secondPlaneParameters[0], secondPlaneParameters[1], secondPlaneParameters[2]);
      gp_Pnt secondPlanePoint = secondPlaneAxes.Location();

      if (firstPlaneDirection.IsEqual(secondPlaneDirection, angleTolerance) && std::abs(firstPlaneParameters[3] - secondPlaneParameters[3]) <= distanceTolerance)
        {
        }
      else if (firstPlaneDirection.IsOpposite(secondPlaneDirection, angleTolerance) && std::abs(firstPlaneParameters[3] + secondPlaneParameters[3]) <= distanceTolerance)
        {
        }
      else
        {
          return Standard_False;
        }
    }
  catch(...)
    {
      return Standard_False;
    }
  return Standard_True;
}
=======

>>>>>>> refactoring of plane comparison

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::isSameEdge(
        const TopoDS_Edge& firstEdge,
        const TopoDS_Edge& secondEdge,
        Standard_Real distanceTolerance){
  // Compare first the start and end points of the curves.
  Standard_Real firstEdgeStart, firstEdgeEnd;
  Handle_Geom_Curve firstCurve = BRep_Tool::Curve(firstEdge, firstEdgeStart, firstEdgeEnd);
  GeomAdaptor_Curve firstCurveAdaptor(firstCurve);

  Standard_Real secondEdgeStart, secondEdgeEnd;
  Handle_Geom_Curve secondCurve = BRep_Tool::Curve(secondEdge, secondEdgeStart, secondEdgeEnd);
  GeomAdaptor_Curve secondCurveAdaptor(secondCurve);

  if (firstCurveAdaptor.GetType() != secondCurveAdaptor.GetType())
    {
      return Standard_False;
    }

  gp_Pnt firstStartPoint, firstEndPoint;
  firstCurve->D0(firstEdgeStart, firstStartPoint);
  firstCurve->D0(firstEdgeEnd, firstEndPoint);

  gp_Pnt secondStartPoint, secondEndPoint;
  secondCurve->D0(secondEdgeStart, secondStartPoint);
  secondCurve->D0(secondEdgeEnd, secondEndPoint);

  if ((firstStartPoint.IsEqual(secondStartPoint, distanceTolerance) && firstEndPoint.IsEqual(secondEndPoint, distanceTolerance)) || (firstStartPoint.IsEqual(secondEndPoint, distanceTolerance) && firstEndPoint.IsEqual(secondStartPoint, distanceTolerance)))
    {
      return Standard_True;
    }
  else
    {
      if ((firstStartPoint.Distance(secondStartPoint) <= distanceTolerance && firstEndPoint.Distance(secondEndPoint) <= distanceTolerance) || ((firstStartPoint.Distance(secondEndPoint) <= distanceTolerance && firstEndPoint.Distance(secondStartPoint) <= distanceTolerance)))
	{
	  return Standard_True;
	}
      else
	{
	  return Standard_False;
	}
    }
}

TopoDS_Face
<<<<<<< HEAD
McCAD::Tools::Preprocessor::Impl::fusePlanes(const TopoDS_Face& firstFace, const TopoDS_Face& secondFace, Standard_Real zeroTolerance, Standard_Real tolerance){
  std::array<Standard_Real, 4> firstUV;
=======
McCAD::Tools::Preprocessor::Impl::fusePlanes(
        const TopoDS_Face& firstFace,
        const TopoDS_Face& secondFace,
        Standard_Real zeroTolerance,
        Standard_Real tolerance){

  std::vector<Standard_Real> firstUV(4);
>>>>>>> refactoring of plane comparison
  BRepTools::UVBounds(firstFace, firstUV[0], firstUV[1], firstUV[2], firstUV[3]);
  std::array<Standard_Real, 4> secondUV;
  BRepTools::UVBounds(secondFace, secondUV[0], secondUV[1], secondUV[2], secondUV[3]);
  for (Standard_Integer i = 0; i <= firstUV.size() - 1; ++i)
    {
      if (firstUV[i] <= zeroTolerance)
	{
	  firstUV[i] = 0.0;
	}
    }
  for (Standard_Integer j = 0; j <= secondUV.size() - 1; ++j)
    {
      if (secondUV[j] <= zeroTolerance)
        {
          secondUV[j] = 0.0;
	}
    }

<<<<<<< HEAD
  std::array<Standard_Real, 4> newUV;
  newUV[0] = (firstUV[0] <= secondUV[0]) ? firstUV[0] : secondUV[0];
  newUV[1] = (firstUV[1] >= secondUV[1]) ? firstUV[1] :	secondUV[1];
  newUV[2] = (firstUV[2] <= secondUV[2]) ? firstUV[2] :	secondUV[2];
  newUV[3] = (firstUV[3] >= secondUV[3]) ? firstUV[3] : secondUV[3];

  Handle_Geom_Surface newSurface = BRep_Tool::Surface(firstFace);
  TopoDS_Face newFace = BRepBuilderAPI_MakeFace(newSurface, newUV[0], newUV[1], newUV[2], newUV[3], tolerance).Face();
=======
  std::vector<Standard_Real> newUV(4);
  newUV[0] = (firstUV[0] <= secondUV[0]) ? firstUV[0] : secondUV[0];
  newUV[1] = (firstUV[1] >= secondUV[1]) ? firstUV[1] : secondUV[1];
  newUV[2] = (firstUV[2] <= secondUV[2]) ? firstUV[2] : secondUV[2];
  newUV[3] = (firstUV[3] >= secondUV[3]) ? firstUV[3] :	secondUV[3];

  TopLoc_Location location;
  Handle_Geom_Surface newSurface = BRep_Tool::Surface(firstFace, location);
  TopoDS_Face newFace
          = BRepBuilderAPI_MakeFace(
              newSurface,
              newUV[0], newUV[1], newUV[2], newUV[3],
                tolerance).Face();
>>>>>>> refactoring of plane comparison

  if (newFace.Orientation() != firstFace.Orientation())
    {
      newFace.Orientation(firstFace.Orientation());
    }
  return newFace;
}
