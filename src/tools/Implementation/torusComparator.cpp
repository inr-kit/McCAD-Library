// McCAD
#include "torusComparator.hpp"
// OCC
#include <gp_Pnt.hxx>
// C++
#include <vector>

McCAD::Tools::TorusComparator::TorusComparator() {}

McCAD::Tools::TorusComparator::TorusComparator(const Standard_Real& precision,
    const Standard_Real& angularTolerance,
    const Standard_Real& distanceTolerance)
    : precision{ precision }, angularTolerance{ angularTolerance },
    distanceTolerance{ distanceTolerance }{
}

McCAD::Tools::TorusComparator::~TorusComparator() {}

std::optional<Standard_Boolean>
McCAD::Tools::TorusComparator::operator()(const GeomAdaptor_Surface& firstAdaptor,
    const GeomAdaptor_Surface& secondAdaptor) {
    auto firstTorus = firstAdaptor.Torus();
    auto secondTorus = secondAdaptor.Torus();
    // Scale torus.
    firstTorus.Scale(gp_Pnt{ 0.0, 0.0, 0.0 }, 0.1);
    secondTorus.Scale(gp_Pnt{ 0.0, 0.0, 0.0 }, 0.1);
    // Compare minorradius of the two torus.
    if (std::abs(firstTorus.MinorRadius() - secondTorus.MinorRadius()) > precision)
        return Standard_False;
    //Compare majorradius of the two torus.
    if (std::abs(firstTorus.MajorRadius() - secondTorus.MajorRadius()) > precision)
        return Standard_False;
    // Compare axis of the two torus.
    if (!firstTorus.Axis().IsParallel(secondTorus.Axis(), angularTolerance) &&
        !firstTorus.Axis().IsOpposite(secondTorus.Axis(), angularTolerance)) {
        return Standard_False;
    }
    // Compare parameters
    return equivalentTorusParameters(firstTorus, firstTorus);
}

std::array<Standard_Real, 35>
McCAD::Tools::TorusComparator::torusParameters(const gp_Torus& torus) const {
    // Coefficients
    TColStd_Array1OfReal torusCoeffs{ 1, 35 };
    torus.Coefficients(torusCoeffs);
    std::array<Standard_Real, 35> torParameters;
    int i = 0;
    for (const auto& coeff : torusCoeffs) {
        torParameters[i] = coeff;
        i++;
    }
    for (auto& parameter : torParameters) {
        if (std::abs(parameter) < precision) parameter = 0.0;
    } 
    return torParameters;
}

std::optional<Standard_Boolean>
McCAD::Tools::TorusComparator::equivalentTorusParameters(const gp_Torus& first,
    const gp_Torus& second) const {
    auto firstTorusParameters = torusParameters(first);
    auto secondTorusParameters = torusParameters(second);
    Standard_Boolean equalityCondition = Standard_True;
    for (Standard_Integer i = 0; i < firstTorusParameters.size(); ++i) {
        if (std::abs(firstTorusParameters[i] - secondTorusParameters[i]) > precision)
            equalityCondition = Standard_False;
    }
    return equalityCondition;
}
