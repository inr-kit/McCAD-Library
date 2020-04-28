// McCAD
#include "tools_impl.hpp"

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
