// McCAD
#include "planeComparator.hpp"
// OCCT
#include <gp_Pnt.hxx>

McCAD::Tools::PlaneComparator::PlaneComparator(){}

McCAD::Tools::PlaneComparator::PlaneComparator(const double& precision,
                                               const double& angularTolerance,
                                               const double& distanceTolerance)
    : precision{precision}, angularTolerance{angularTolerance},
      distanceTolerance{distanceTolerance}{
}

McCAD::Tools::PlaneComparator::~PlaneComparator(){}

/** ********************************************************************
* @brief    An operator that compares two OCCT planes.
* @param    firstAdaptor is a OCCT GeomAdaptor_Surface object.
* @param    secondAdaptor is a OCCT GeomAdaptor_Surface object.
* @return   true if the two are equal.
* @date     24/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::optional<bool>
McCAD::Tools::PlaneComparator::operator()(const GeomAdaptor_Surface& firstAdaptor,
                                          const GeomAdaptor_Surface& secondAdaptor){
    auto firstPlane = firstAdaptor.Plane();
    auto secondPlane = secondAdaptor.Plane();
    // Scale planes
    firstPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    secondPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    // Compare distance between planes.
    if (firstPlane.Distance(secondPlane) >= distanceTolerance){
        return false;
    }
    // Compare plane coefficients.
    return equivalentPlaneCoefficients(firstPlane, secondPlane);
}

/** ********************************************************************
* @brief    A function that calculates the UV coefficients of a plane.
* @param    plane is a OCCT plane surface.
* @return   an array of the coefficients of the plane.
* @date     24/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::array<double, 4>
McCAD::Tools::PlaneComparator::planeCoefficients(const gp_Pln& plane) const{
    std::array<double, 4> planeCoefficients;
    plane.Coefficients(planeCoefficients[0], planeCoefficients[1],
                       planeCoefficients[2], planeCoefficients[3]);
    for(auto& coeff : planeCoefficients){
        if(std::abs(coeff) < precision) coeff = 0.0;
    }
    return planeCoefficients;
}

/** ********************************************************************
* @brief    A function that compares the UV coefficients of two planes.
* @param    first is a OCCT plane surface.
* @param    second is a OCCT plane surface.
* @return   true if the two planes have equal coefficients.
* @date     24/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::optional<bool>
McCAD::Tools::PlaneComparator::equivalentPlaneCoefficients(const gp_Pln& first,
                                                           const gp_Pln& second) const{
    auto firstPlaneCoefficients = planeCoefficients(first);
    gp_Dir firstPlaneDirection{
        firstPlaneCoefficients[0],
        firstPlaneCoefficients[1],
        firstPlaneCoefficients[2]
    };
    auto secondPlaneCoefficients = planeCoefficients(second);
    gp_Dir secondPlaneDirection{
        secondPlaneCoefficients[0],
        secondPlaneCoefficients[1],
        secondPlaneCoefficients[2]
    };
    return (firstPlaneDirection.IsEqual(secondPlaneDirection, angularTolerance)
            && std::abs(firstPlaneCoefficients[3] - secondPlaneCoefficients[3])
            < distanceTolerance) ||
           (firstPlaneDirection.IsOpposite(secondPlaneDirection, angularTolerance)
            && std::abs(firstPlaneCoefficients[3] + secondPlaneCoefficients[3])
            < distanceTolerance);
}
