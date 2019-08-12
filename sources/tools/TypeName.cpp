#include "TypeName.hpp"

std::string
McCAD::Tools::toTypeName(
        GeomAbs_SurfaceType surfaceType){
    switch(surfaceType){
    case GeomAbs_Plane:
        return "Plane";
    case GeomAbs_Cylinder:
        return "Cylinder";
    case GeomAbs_Cone:
        return "Cone";
    case GeomAbs_Sphere:
        return "Sphere";
    case GeomAbs_Torus:
        return "Torus";
    case GeomAbs_BezierSurface:
        return "Bezier surface";
    case GeomAbs_BSplineSurface:
        return "B-spline surface";
    case GeomAbs_SurfaceOfRevolution:
        return "Surface of revolution";
    case GeomAbs_SurfaceOfExtrusion:
        return "Surface of extrusion";
    case GeomAbs_OffsetSurface:
        return "Offset surface";
    default:
        return "Other surface";
    }
}

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
        return "Other curve";
    }
}
