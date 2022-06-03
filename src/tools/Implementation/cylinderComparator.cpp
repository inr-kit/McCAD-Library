// McCAD
#include "cylinderComparator.hpp"
// OCC
#include <gp_Pnt.hxx>

McCAD::Tools::CylinderComparator::CylinderComparator() {}

McCAD::Tools::CylinderComparator::CylinderComparator(const Standard_Real& precision,
                                                     const Standard_Real& angularTolerance,
                                                     const Standard_Real& distanceTolerance)
    : precision{precision}, angularTolerance{angularTolerance},
      distanceTolerance{distanceTolerance}{
}

McCAD::Tools::CylinderComparator::~CylinderComparator(){}

std::optional<Standard_Boolean>
McCAD::Tools::CylinderComparator::operator()(const GeomAdaptor_Surface& firstAdaptor,
                                             const GeomAdaptor_Surface& secondAdaptor){
    auto firstCyl = firstAdaptor.Cylinder();
    auto secondCyl = secondAdaptor.Cylinder();
    // Scale cylinders
    firstCyl.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    secondCyl.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    // Compare radii of the two cylinders.
    if (std::abs(firstCyl.Radius() - secondCyl.Radius()) > precision)
        return Standard_False;
    // Compare axis of the two cylinders.
    if (!firstCyl.Axis().IsParallel(secondCyl.Axis(), angularTolerance) &&
            !firstCyl.Axis().IsOpposite(secondCyl.Axis(), angularTolerance)){
        return Standard_False;
    }
    // Compare parameters
    return equivalentCylParameters(firstCyl, secondCyl);
}

std::array<Standard_Real, 10>
McCAD::Tools::CylinderComparator::cylParameters(const gp_Cylinder& cylinder) const{
    std::array<Standard_Real, 10> cylParameters;
    cylinder.Coefficients(cylParameters[0], cylParameters[1], cylParameters[2],
                          cylParameters[3], cylParameters[4], cylParameters[5],
                          cylParameters[6], cylParameters[7], cylParameters[8],
                          cylParameters[9]);
    for(auto& parameter : cylParameters){
        if(std::abs(parameter) < precision) parameter = 0.0;
    }
    return cylParameters;
}

std::optional<Standard_Boolean>
McCAD::Tools::CylinderComparator::equivalentCylParameters(const gp_Cylinder& first,
                                                          const gp_Cylinder& second) const{
    auto firstCylParameters = cylParameters(first);
    auto secondCylParameters = cylParameters(second);
    Standard_Boolean equalityCondition = Standard_True;
    for(Standard_Integer i = 0; i < firstCylParameters.size(); ++i){
        if(std::abs(firstCylParameters[i] - secondCylParameters[i]) > precision)
            equalityCondition = Standard_False;
    }
    return equalityCondition;
}
