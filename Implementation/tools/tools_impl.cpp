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

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::isSameEdge(const TopoDS_Edge& firstEdge,
                                             const TopoDS_Edge& secondEdge,
                                             Standard_Real angularTolerance,
                                             Standard_Real distanceTolerance){
    // Compare first the start and end points of the curves.
    Standard_Real firstEdgeStart, firstEdgeEnd;
    TopLoc_Location firstLocation;
    Handle_Geom_Curve firstCurve = BRep_Tool::Curve(firstEdge, firstLocation,
                                                    firstEdgeStart, firstEdgeEnd);
    GeomAdaptor_Curve firstCurveAdaptor(firstCurve);

    Standard_Real secondEdgeStart, secondEdgeEnd;
    TopLoc_Location secondLocation;
    Handle_Geom_Curve secondCurve = BRep_Tool::Curve(secondEdge, secondLocation,
                                                     secondEdgeStart, secondEdgeEnd);
    GeomAdaptor_Curve secondCurveAdaptor(secondCurve);
    // Compare types
    if (firstCurveAdaptor.GetType() != secondCurveAdaptor.GetType()){
        //std::cout << "Edges type different" << std::endl;
        return Standard_False;
    }
    // Compare locations
    if (firstLocation != secondLocation){
        //std::cout << "Location different" << std::endl;
        return Standard_False;
    }
    if ((firstEdgeStart != secondEdgeStart) || (firstEdgeEnd != secondEdgeEnd)){
        //std::cout << "Edges start or end different" << std::endl;
        return Standard_False;
    }
    if (firstCurveAdaptor.GetType() == GeomAbs_Line){
        if (firstCurveAdaptor.Line().Distance(secondCurveAdaptor.Line()) >
                distanceTolerance){
            return Standard_False;
        }
        if (firstCurveAdaptor.Line().Angle(secondCurveAdaptor.Line()) >
                angularTolerance){
            return Standard_False;
        }
    }
    /* //debug
    std::cout << "Edges equal" << std::endl;
    std::cout << "Distance betwen curves: " <<
                 firstCurveAdaptor.Line().Distance(secondCurveAdaptor.Line()) <<
                 ", tolerance: " << distanceTolerance << std::endl;
    std::cout << "Angle betwen curves: " <<
                 firstCurveAdaptor.Line().Angle(secondCurveAdaptor.Line()) <<
                 ", tolerance: " << 1.0e-3 * M_PI << std::endl;
    */ //debug
    return Standard_True;
}
