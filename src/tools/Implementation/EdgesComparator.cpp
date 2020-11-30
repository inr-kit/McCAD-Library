// McCAD
#include "EdgesComparator.hpp"
// OCC
#include <gp_Lin.hxx>
#include <Geom_Curve.hxx>
#include <BRep_Tool.hxx>
#include <GeomAdaptor_Curve.hxx>

Standard_Boolean
McCAD::Tools::EdgesComparator::operator()(const TopoDS_Edge& firstEdge,
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
