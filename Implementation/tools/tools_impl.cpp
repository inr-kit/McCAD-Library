// McCAD
#include "tools_impl.hpp"

void
McCAD::Tools::Preprocessor::Impl::checkBndSurfaces(const TopoDS_Solid& solid,
                                                   Standard_Boolean& isTorus,
                                                   Standard_Boolean& isSpline){
  for(const auto& face : ShapeView<TopAbs_FACE>{solid}){
      GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
      if(surfAdaptor.GetType() == GeomAbs_Torus){
          std::cout << "    -- The current verion doesn't support processing of"
                       "Tori. Ignoring solid!" << std::endl;
          isTorus = Standard_True;
      } else if (surfAdaptor.GetType() == GeomAbs_BSplineSurface){
            std::cout << "    -- The current verion doesn't support processing "
                         "of splines. Ignoring solid!" << std::endl;
            isSpline = Standard_True;
      }
    }
}

void
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape,
						   Standard_Real precision,
						   Standard_Real maxTolerance){
  Handle_ShapeFix_FixSmallFace smallFaceFix = new ShapeFix_FixSmallFace;
  smallFaceFix->Init(solidShape);
  smallFaceFix->SetPrecision(precision);
  smallFaceFix->SetMaxTolerance(maxTolerance);
  smallFaceFix->Perform();
  solidShape = smallFaceFix->FixShape();
}

void
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid){
  Handle_ShapeFix_Solid solidFix = new ShapeFix_Solid(solid);
  solidFix->Perform();
  solid = TopoDS::Solid(solidFix->Solid());
}

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkFace(const TopoDS_Face& face,
					    Standard_Real tolerance){
  ShapeAnalysis_CheckSmallFace shapeAnalysis;
  TopoDS_Edge edge1, edge2;
  if( shapeAnalysis.CheckSpotFace(face, tolerance) ||
      shapeAnalysis.CheckStripFace(face, edge1, edge2, tolerance))
    {
      return Standard_True;
    }
  return Standard_False;
}

void
McCAD::Tools::Preprocessor::Impl::fixFace(TopoDS_Face& face,
					  Standard_Real precision,
					  Standard_Real maxTolerance){
  Handle_ShapeFix_Shape shapeFixer = new ShapeFix_Shape(face);
  shapeFixer->SetPrecision(precision);
  shapeFixer->SetMaxTolerance(maxTolerance);
  shapeFixer->FixWireTool()->FixRemovePCurveMode() = 1;
  shapeFixer->FixWireTool()->FixConnected();
  shapeFixer->Perform();
  face = TopoDS::Face(shapeFixer->Shape());
}

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::isSameEdge(const TopoDS_Edge& firstEdge,
					     const TopoDS_Edge& secondEdge,
					     Standard_Real distanceTolerance){
  // Compare first the start and end points of the curves.
  Standard_Real firstEdgeStart, firstEdgeEnd;
  Handle_Geom_Curve firstCurve = BRep_Tool::Curve(firstEdge, firstEdgeStart,
						  firstEdgeEnd);
  GeomAdaptor_Curve firstCurveAdaptor(firstCurve);

  Standard_Real secondEdgeStart, secondEdgeEnd;
  Handle_Geom_Curve secondCurve = BRep_Tool::Curve(secondEdge, secondEdgeStart,
						   secondEdgeEnd);
  GeomAdaptor_Curve secondCurveAdaptor(secondCurve);

  if (firstCurveAdaptor.GetType() != secondCurveAdaptor.GetType())
    {
      return Standard_False;
    }
  if ((firstEdgeStart != secondEdgeStart) || (firstEdgeEnd != secondEdgeEnd))
    {
      return Standard_False;
    }
  if (firstCurveAdaptor.GetType() == GeomAbs_Line)
    {
      if (firstCurveAdaptor.Line().Distance(secondCurveAdaptor.Line()) > distanceTolerance)
	{
	  return Standard_False;
	}
    }
  return Standard_True;
}
