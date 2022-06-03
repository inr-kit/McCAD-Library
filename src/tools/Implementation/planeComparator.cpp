// McCAD
#include "planeComparator.hpp"
// OCC
#include <gp_Pnt.hxx>

McCAD::Tools::PlaneComparator::PlaneComparator(){}

McCAD::Tools::PlaneComparator::PlaneComparator(const Standard_Real& precision,
                                               const Standard_Real& angularTolerance,
                                               const Standard_Real& distanceTolerance)
    : precision{precision}, angularTolerance{angularTolerance},
      distanceTolerance{distanceTolerance}{
}

McCAD::Tools::PlaneComparator::~PlaneComparator(){}

std::optional<Standard_Boolean>
McCAD::Tools::PlaneComparator::operator()(const GeomAdaptor_Surface& firstAdaptor,
                                          const GeomAdaptor_Surface& secondAdaptor){
    auto firstPlane = firstAdaptor.Plane();
    auto secondPlane = secondAdaptor.Plane();
    // Scale planes
    firstPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    secondPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    // Compare distance between planes.
    if (firstPlane.Distance(secondPlane) > distanceTolerance){
        return Standard_False;
    }
    // Compare plane parameters
    return equivalentPlaneParameters(firstPlane, secondPlane);
}

std::array<Standard_Real, 4>
McCAD::Tools::PlaneComparator::planeParameters(const gp_Pln& plane) const{
    std::array<Standard_Real, 4> planeParameters;
    plane.Coefficients(planeParameters[0], planeParameters[1],
            planeParameters[2], planeParameters[3]);
    for(auto& parameter : planeParameters){
        if(std::abs(parameter) < precision) parameter = 0.0;
    }
    return planeParameters;
}

std::optional<Standard_Boolean>
McCAD::Tools::PlaneComparator::equivalentPlaneParameters(const gp_Pln& first,
                                                         const gp_Pln& second) const{
    auto firstPlaneParameters = planeParameters(first);
    gp_Dir firstPlaneDirection{
        firstPlaneParameters[0],
        firstPlaneParameters[1],
        firstPlaneParameters[2]
    };
    auto secondPlaneParameters = planeParameters(second);
    gp_Dir secondPlaneDirection{
        secondPlaneParameters[0],
        secondPlaneParameters[1],
        secondPlaneParameters[2]
    };
    return (firstPlaneDirection.IsEqual(secondPlaneDirection, angularTolerance)
            && std::abs(firstPlaneParameters[3] - secondPlaneParameters[3])
            < distanceTolerance) ||
           (firstPlaneDirection.IsOpposite(secondPlaneDirection, angularTolerance)
            && std::abs(firstPlaneParameters[3] + secondPlaneParameters[3])
            < distanceTolerance);
}
