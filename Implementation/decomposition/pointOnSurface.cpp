//C++
//McCAD
#include "pointOnSurface.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "senseEvaluateor.hpp"
//OCC
#include <BRepTools.hxx>
//#include "BRepAdaptor_Surface.hxx"
#include "Extrema_ExtPS.hxx"

Standard_Boolean
McCAD::Decomposition::PointOnSurface::operator()(
        const TopoDS_Face& face, const gp_Pnt& aPoint,
        const Standard_Real& distanceTolerance){
    BRepAdaptor_Surface surfaceAdaptor(face, Standard_True);
    Standard_Real uvTolerance = surfaceAdaptor.Tolerance();
    std::array<Standard_Real, 4> uvParameters;
    uvParameters[0] = surfaceAdaptor.FirstUParameter();
    uvParameters[1] = surfaceAdaptor.LastUParameter();
    uvParameters[2] = surfaceAdaptor.FirstVParameter();
    uvParameters[3] = surfaceAdaptor.LastVParameter();
    Extrema_ExtPS extremumDistances(aPoint, surfaceAdaptor, uvParameters[0],
            uvParameters[1], uvParameters[2], uvParameters[3], uvTolerance,
            uvTolerance);
    if (extremumDistances.IsDone() && extremumDistances.NbExt() != 0){
        return (extremumDistances.SquareDistance(1) < distanceTolerance * distanceTolerance);
    }
    return Standard_False;
}
