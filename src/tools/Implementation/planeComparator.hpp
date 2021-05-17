#ifndef PLANECOMPARATOR_HPP
#define PLANECOMPARATOR_HPP

// C++
#include <array>
#include <optional>
#include <cmath>
// McCAD
#include <Standard.hxx>
// OCC
#include <gp_Pln.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>

namespace McCAD::Tools{
  class PlaneComparator{
  public:
   PlaneComparator(Standard_Real parameterTolerance = 1.0e-7,
                   Standard_Real angularTolerance = 1.0e-3 * M_PI,
                   Standard_Real distanceTolerance = 1.0e-5);
   std::optional<Standard_Boolean> operator()(
           const GeomAdaptor_Surface& firstAdaptor,
           const GeomAdaptor_Surface& secondAdaptor);
  private:
    Standard_Real parameterTolerance;
    Standard_Real angularTolerance;
    Standard_Real distanceTolerance;

    std::array<Standard_Real, 4> planeParameters(const gp_Pln& plane) const;
    Standard_Boolean equivalentPlaneParameters(const gp_Pln& first,
                                               const gp_Pln& second) const;
    };
}

#endif //PLANECOMPARATOR_HPP
