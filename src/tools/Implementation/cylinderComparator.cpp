// McCAD
#include "cylinderComparator.hpp"
// OCC
#include <gp_Pnt.hxx>

McCAD::Tools::CylinderComparator::CylinderComparator() {}

McCAD::Tools::CylinderComparator::CylinderComparator(const double& precision,
                                                     const double& angularTolerance,
                                                     const double& distanceTolerance)
    : precision{precision}, angularTolerance{angularTolerance},
      distanceTolerance{distanceTolerance}{
}

McCAD::Tools::CylinderComparator::~CylinderComparator(){}

/** ********************************************************************
* @brief    An operator that compares two OCCT cylinders.
* @param    firstAdaptor is a OCCT GeomAdaptor_Surface object.
* @param    secondAdaptor is a OCCT GeomAdaptor_Surface object.
* @return   true if the two are equal.
* @date     24/08/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<bool>
McCAD::Tools::CylinderComparator::operator()(const GeomAdaptor_Surface& firstAdaptor,
                                             const GeomAdaptor_Surface& secondAdaptor){
    auto firstCyl = firstAdaptor.Cylinder();
    auto secondCyl = secondAdaptor.Cylinder();
    // Scale cylinders
    firstCyl.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    secondCyl.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    // Compare radii of the two cylinders.
    if (std::abs(firstCyl.Radius() - secondCyl.Radius()) >= precision)
        return false;
    // Compare axis of the two cylinders.
    if (!firstCyl.Axis().IsParallel(secondCyl.Axis(), angularTolerance) &&
            !firstCyl.Axis().IsOpposite(secondCyl.Axis(), angularTolerance)){
        return false;
    }
    // Compare parameters
    return equivalentCylCoefficients(firstCyl, secondCyl);
}

/** ********************************************************************
* @brief    A function that calculates the coefficients of a cylinder.
* @param    cone is a OCCT cone surface.
* @return   an array of the coefficients of the cone.
* @date     24/08/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::array<double, 10>
McCAD::Tools::CylinderComparator::cylCoefficients(const gp_Cylinder& cylinder) const{
    std::array<double, 10> cylCoefficients;
    cylinder.Coefficients(cylCoefficients[0], cylCoefficients[1], cylCoefficients[2],
                          cylCoefficients[3], cylCoefficients[4], cylCoefficients[5],
                          cylCoefficients[6], cylCoefficients[7], cylCoefficients[8],
                          cylCoefficients[9]);
    for(auto& coeff : cylCoefficients){
        if(std::abs(coeff) < precision) coeff = 0.0;
    }
    return cylCoefficients;
}

/** ********************************************************************
* @brief    A function that compares the coefficients of two cylinders.
* @param    first is a OCCT cylinder surface.
* @param    second is a OCCT cylinder surface.
* @return   true if the two cylinders have equal coefficients.
* @date     24/08/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<bool>
McCAD::Tools::CylinderComparator::equivalentCylCoefficients(const gp_Cylinder& first,
                                                            const gp_Cylinder& second) const{
    auto firstCylCoefficients = cylCoefficients(first);
    auto secondCylCoefficients = cylCoefficients(second);
    bool equalityCondition = true;
    for(int i = 0; i < firstCylCoefficients.size(); ++i){
        if(std::abs(firstCylCoefficients[i] - secondCylCoefficients[i]) >= precision)
            equalityCondition = false;
    }
    return equalityCondition;
}
