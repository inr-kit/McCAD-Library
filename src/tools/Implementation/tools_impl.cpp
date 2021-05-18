// McCAD
#include "tools_impl.hpp"
#include "ShapeView.hpp"
// OCC
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Solid.hxx>
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Solid.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopoDS.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <Geom_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRepGProp.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <TopoDS_Edge.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <ElSLib.hxx>
#include <CSLib_DerivativeStatus.hxx>
#include <CSLib.hxx>
#include <Precision.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <TopAbs_Orientation.hxx>
#include <TopLoc_Location.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax3.hxx>
#include <Geom_Curve.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepTools.hxx>

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
            shapeAnalysis.CheckStripFace(face, edge1, edge2, tolerance)){
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
