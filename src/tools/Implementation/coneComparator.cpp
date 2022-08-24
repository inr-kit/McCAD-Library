// McCAD
#include "coneComparator.hpp"
// OCC
#include <gp_Pnt.hxx>

McCAD::Tools::ConeComparator::ConeComparator() {}

McCAD::Tools::ConeComparator::ConeComparator(const double& precision,
                                             const double& angularTolerance,
                                             const double& distanceTolerance)
    : precision{precision}, angularTolerance{angularTolerance},
      distanceTolerance{distanceTolerance}{
}

McCAD::Tools::ConeComparator::~ConeComparator(){}

/** ********************************************************************
* @brief    An operator that compares two OCCT cones.
* @param    firstAdaptor is a OCCT GeomAdaptor_Surface object.
* @param    secondAdaptor is a OCCT GeomAdaptor_Surface object.
* @return   true if the two are equal.
* @date     24/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::optional<bool>
McCAD::Tools::ConeComparator::operator()(const GeomAdaptor_Surface& firstAdaptor,
                                         const GeomAdaptor_Surface& secondAdaptor){
    auto firstCone = firstAdaptor.Cone();
    auto secondCone = secondAdaptor.Cone();
    // Scale cones
    firstCone.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    secondCone.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    // Compare reference radii of the two cones.
    if (std::abs(firstCone.RefRadius() - secondCone.RefRadius()) >= precision)
        return false;
    // Compare symmetry axes of the two cones.
    if (!firstCone.Axis().IsParallel(secondCone.Axis(), angularTolerance) &&
            !firstCone.Axis().IsOpposite(secondCone.Axis(), angularTolerance)){
        return false;
    }
    // Compare Apex.
    if (!firstCone.Apex().IsEqual(secondCone.Apex(), distanceTolerance)) {
        return false;
    }
    // Compare semiangle.
    if (std::abs(firstCone.SemiAngle() - secondCone.SemiAngle()) >= precision){
        return false;
    }
    // Compare parameters
    return equivalentConCoefficients(firstCone, secondCone);
}

/** ********************************************************************
* @brief    A function that calculates the coefficients of a cone.
* @param    cone is a OCCT cone surface.
* @return   an array of the coefficients of the cone.
* @date     24/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::array<double, 10>
McCAD::Tools::ConeComparator::conCoefficients(const gp_Cone& cone) const{
    std::array<double, 10> conCoefficients;
    cone.Coefficients(conCoefficients[0], conCoefficients[1], conCoefficients[2],
                      conCoefficients[3], conCoefficients[4], conCoefficients[5],
                      conCoefficients[6], conCoefficients[7], conCoefficients[8],
                      conCoefficients[9]);
    for(auto& Coeff : conCoefficients){
        if(std::abs(Coeff) < precision) Coeff = 0.0;
    }
    return conCoefficients;
}

/** ********************************************************************
* @brief    A function that compares the UV coefficients of two cones.
* @param    first is a OCCT cone surface.
* @param    second is a OCCT cone surface.
* @return   true if the two cones have equal coefficients.
* @date     24/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::optional<bool>
McCAD::Tools::ConeComparator::equivalentConCoefficients(const gp_Cone& first,
                                                        const gp_Cone& second) const{
    auto firstConCoefficients = conCoefficients(first);
    auto secondConCoefficients = conCoefficients(second);
    bool equalityCondition = true;
    for(int i = 0; i < firstConCoefficients.size(); ++i){
        if(std::abs(firstConCoefficients[i] - secondConCoefficients[i]) >= precision)
            equalityCondition = false;
    }
    return equalityCondition;
}
