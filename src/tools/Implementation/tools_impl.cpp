// McCAD
#include "tools_impl.hpp"
// OCC
#include <Message_ProgressRange.hxx>
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Solid.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <ShapeFix_Shape.hxx>

McCAD::Tools::Preprocessor::Impl::Impl(){}

McCAD::Tools::Preprocessor::Impl::Impl(const double& precision,
                                       const double& faceTolerance) :
    precision{precision}, maxTolerance{faceTolerance}{}

McCAD::Tools::Preprocessor::Impl::~Impl(){}

/** ********************************************************************
* @brief    A function that uses OCCT tools to remove small faces.
* @param    solidShape is a OCCT shape.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(TopoDS_Shape& solidShape){
    Handle_ShapeFix_FixSmallFace smallFaceFix = new ShapeFix_FixSmallFace;
    smallFaceFix->Init(solidShape);
    smallFaceFix->SetPrecision(precision);
    smallFaceFix->SetMaxTolerance(maxTolerance);
    smallFaceFix->Perform();
    solidShape = smallFaceFix->FixShape();
}

/** ********************************************************************
* @brief    A function that uses OCCT ShapeFix tool to fix solids.
* @param    solid is a OCCT solid.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid){
    Handle_ShapeFix_Solid solidFix = new ShapeFix_Solid(solid);
    solidFix->SetPrecision(precision);
    solidFix->Perform();
    solid = TopoDS::Solid(solidFix->Solid());
}

/** ********************************************************************
* @brief    A function that checks that the face isn't a strip or spot one.
* @param    face is a OCCT face.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
bool
McCAD::Tools::Preprocessor::Impl::checkFace(const TopoDS_Face& face){
    ShapeAnalysis_CheckSmallFace shapeAnalysis;
    TopoDS_Edge edge1, edge2;
    if( shapeAnalysis.CheckSpotFace(face, maxTolerance) ||
            shapeAnalysis.CheckStripFace(face, edge1, edge2, maxTolerance)){
        return true;
    }
    return false;
}

/** ********************************************************************
* @brief    A function that fixes a face.
* @param    face is a OCCT face.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
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
