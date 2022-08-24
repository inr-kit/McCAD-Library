#include "CurveUtilities.hpp"

/** ********************************************************************
* @brief    A function that returns a string name of a given OCCT curve type.
* @param    curveType is a OCCT curve type.
* @return   string name of the given curve type.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::string
McCAD::Tools::toTypeName(
        GeomAbs_CurveType curveType){
    switch(curveType){
    case GeomAbs_Line:
        return "Line";
    case GeomAbs_Circle:
        return "Circle";
    case GeomAbs_Ellipse:
        return "Ellipse";
    case GeomAbs_Hyperbola:
        return "Hyperbola";
    case GeomAbs_Parabola:
        return "Parabola";
    case GeomAbs_BezierCurve:
        return "Bezier curve";
    case GeomAbs_BSplineCurve:
        return "B-spline curve";
    case GeomAbs_OffsetCurve:
        return "Offset curve";
    default:
        return "unknown curve";
    }
}
