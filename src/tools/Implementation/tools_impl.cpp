// McCAD
#include "tools_impl.hpp"
// OCC
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Solid.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <ShapeFix_Shape.hxx>

McCAD::Tools::Preprocessor::Impl::Impl(){}

McCAD::Tools::Preprocessor::Impl::Impl(const Standard_Real& precision,
                                       const Standard_Real& faceTolerance) :
    precision{precision}, maxTolerance{faceTolerance}{}

McCAD::Tools::Preprocessor::Impl::~Impl(){}

void
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape){
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
    solidFix->SetPrecision(precision);
    solidFix->Perform();
    solid = TopoDS::Solid(solidFix->Solid());
}

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkFace(const TopoDS_Face& face){
    ShapeAnalysis_CheckSmallFace shapeAnalysis;
    TopoDS_Edge edge1, edge2;
    if( shapeAnalysis.CheckSpotFace(face, maxTolerance) ||
            shapeAnalysis.CheckStripFace(face, edge1, edge2, maxTolerance)){
        return Standard_True;
    }
    return Standard_False;
}

void
McCAD::Tools::Preprocessor::Impl::fixFace(TopoDS_Face& face){
    Handle_ShapeFix_Shape shapeFixer = new ShapeFix_Shape(face);
    shapeFixer->SetPrecision(precision);
    shapeFixer->SetMaxTolerance(maxTolerance);
    shapeFixer->FixWireTool()->FixRemovePCurveMode() = 1;
    shapeFixer->FixWireTool()->FixConnected();
    shapeFixer->Perform();
    face = TopoDS::Face(shapeFixer->Shape());
}
