#include "SurfaceUtilities.hpp"

std::string
McCAD::Tools::toTypeName(GeomAbs_SurfaceType surfaceType){
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
        return "Unknown surface";
    }
}
