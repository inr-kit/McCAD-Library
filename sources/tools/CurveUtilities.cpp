#include "CurveUtilities.hpp"

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
	//case GeomAbs_OffsetCurve:
        //return "Offset curve";
    default:
        return "Other curve";
    }
}
