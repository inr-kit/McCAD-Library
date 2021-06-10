#ifndef PLANECOMPARATOR_HPP
#define PLANECOMPARATOR_HPP

// C++
#include <array>
#include <optional>
// OCC
#include <Standard.hxx>
#include <gp_Pln.hxx>
#include <GeomAdaptor_Surface.hxx>

namespace McCAD::Tools{
  class PlaneComparator{
  public:
      PlaneComparator();
      PlaneComparator(const Standard_Real& precision, const Standard_Real& angularTolerance,
                      const Standard_Real& distanceTolerance);
      ~PlaneComparator();
  private:
      Standard_Real precision{1.0e-7};
      Standard_Real angularTolerance{1.0e-4};
      Standard_Real distanceTolerance{1.0e-5};
  public:
      std::optional<Standard_Boolean> operator()(const GeomAdaptor_Surface& firstAdaptor,
                                                 const GeomAdaptor_Surface& secondAdaptor);
      std::array<Standard_Real, 4> planeParameters(const gp_Pln& plane) const;
      Standard_Boolean equivalentPlaneParameters(const gp_Pln& first,
                                                 const gp_Pln& second) const;
    };
}

#endif //PLANECOMPARATOR_HPP
