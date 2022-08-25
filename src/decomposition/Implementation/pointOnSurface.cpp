// C++
#include <array>
//McCAD
#include "pointOnSurface.hpp"
//OCC
#include "BRepAdaptor_Surface.hxx"
#include "Extrema_ExtPS.hxx"

/** ********************************************************************
* @brief    Afunction that detects if a point is on a given surface.
* @param    face is a OCCT face.
* @param    aPoint is a OCCT point.
* @param    distanceTolerance is used to compare the distance and is set in inputConfig file.
* @return   true if the point is on the surface.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::PointOnSurface::operator()(const TopoDS_Face& face, 
    const gp_Pnt& aPoint, const double& distanceTolerance){
    BRepAdaptor_Surface surfaceAdaptor(face, true);
    double uvTolerance = surfaceAdaptor.Tolerance();
    std::array<double, 4> uvParameters;
    uvParameters[0] = surfaceAdaptor.FirstUParameter();
    uvParameters[1] = surfaceAdaptor.LastUParameter();
    uvParameters[2] = surfaceAdaptor.FirstVParameter();
    uvParameters[3] = surfaceAdaptor.LastVParameter();
    Extrema_ExtPS extremumDistances(aPoint, surfaceAdaptor, uvParameters[0],
            uvParameters[1], uvParameters[2], uvParameters[3], uvTolerance,
            uvTolerance);
    if (extremumDistances.IsDone() && extremumDistances.NbExt() != 0){
        return (extremumDistances.SquareDistance(1) < std::pow(distanceTolerance, 2));
    }
    return false;
}
