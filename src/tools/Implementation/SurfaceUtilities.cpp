// C++
#include <list>
// McCAD
#include "SurfaceUtilities.hpp"

/** ********************************************************************
* @brief   A function that maps a surface type to a string.
* @param   surfaceType is a OCCT surface type, GeomAbs_SurfaceType.
* @return  string.
* @date    31/12/2020
* @author  Moataz Harb & Christian Wegmann
* **********************************************************************/
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

/** ********************************************************************
* @brief   A function that checks if a surface type is supported by McCAD.
* @param   surfaceType is a OCCT surface type, GeomAbs_SurfaceType.
* @param   bool.
* @date    31/12/2020
* @author  Moataz Harb & Christian Wegmann
* **********************************************************************/
bool
McCAD::Tools::checkSupported(GeomAbs_SurfaceType surfaceType) {
    // Create a list of supported surfaces.
    std::list<GeomAbs_SurfaceType> supportedSurfaces = { GeomAbs_Plane , GeomAbs_Cylinder , GeomAbs_Torus };
    if (std::find(supportedSurfaces.begin(), supportedSurfaces.end(), surfaceType) != supportedSurfaces.end()) return true;
    else return false;
}
