// McCAD
#include "coneComparator.hpp"
// OCC
#include <gp_Pnt.hxx>

McCAD::Tools::ConeComparator::ConeComparator() {}

McCAD::Tools::ConeComparator::ConeComparator(const Standard_Real& precision,
    const Standard_Real& angularTolerance,
    const Standard_Real& distanceTolerance)
    : precision{ precision }, angularTolerance{ angularTolerance },
    distanceTolerance{ distanceTolerance }{
}

McCAD::Tools::ConeComparator::~ConeComparator() {}

std::optional<Standard_Boolean>
McCAD::Tools::ConeComparator::operator()(const GeomAdaptor_Surface& firstAdaptor,
    const GeomAdaptor_Surface& secondAdaptor) {
    auto firstCone = firstAdaptor.Cone();
    auto secondCone = secondAdaptor.Cone();
    // Scale cones
    firstCone.Scale(gp_Pnt{ 0.0, 0.0, 0.0 }, 0.1);
    secondCone.Scale(gp_Pnt{ 0.0, 0.0, 0.0 }, 0.1);
    // Compare radii of the two cones.
    if (std::abs(firstCone.RefRadius() - secondCone.RefRadius()) > precision)
        return Standard_False;
    // Compare semiangle of two cones.
    if (std::abs(pow(tan(firstCone.SemiAngle()), 2.0) - pow(tan(secondCone.SemiAngle()), 2.0)) > precision)
        return Standard_False;
    // Compare symmetry axis of the two cones.
    if (!firstCone.Axis().IsParallel(secondCone.Axis(), angularTolerance) &&
        !firstCone.Axis().IsOpposite(secondCone.Axis(), angularTolerance)) {
        return Standard_False;
    }
    // Compare direction of two cones.
    if (!firstCone.Position().Direction().IsParallel(secondCone.Position().Direction(), angularTolerance))
        return Standard_False;
    // Compare parameters
    return equivalentConeParameters(firstCone, secondCone);
}

std::array<Standard_Real, 10>
McCAD::Tools::ConeComparator::coneParameters(const gp_Cone& cone) const {
    std::array<Standard_Real, 10> coneParameters;
    cone.Coefficients(coneParameters[0], coneParameters[1], coneParameters[2],
        coneParameters[3], coneParameters[4], coneParameters[5],
        coneParameters[6], coneParameters[7], coneParameters[8],
        coneParameters[9]);
    for (auto& parameter : coneParameters) {
        if (std::abs(parameter) < precision) parameter = 0.0;
    }
    return coneParameters;
}

std::optional<Standard_Boolean>
McCAD::Tools::ConeComparator::equivalentConeParameters(const gp_Cone& first,
    const gp_Cone& second) const {
    auto firstConeParameters = coneParameters(first);
    auto secondConeParameters = coneParameters(second);
    Standard_Boolean equalityCondition = Standard_True;
    for (Standard_Integer i = 0; i < firstConeParameters.size(); ++i) {
        if (std::abs(firstConeParameters[i] - secondConeParameters[i]) > precision)
            equalityCondition = Standard_False;
    }
    return equalityCondition;
}
