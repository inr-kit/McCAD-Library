#ifndef PLANECOMPARATOR_HPP
#define PLANECOMPARATOR_HPP

// C++
#include <array>
#include <optional>
// OCCT
#include <gp_Pln.hxx>
#include <GeomAdaptor_Surface.hxx>

namespace McCAD::Tools{
  class PlaneComparator{
  public:
      PlaneComparator();
      PlaneComparator(const double& precision, const double& angularTolerance,
                      const double& distanceTolerance);
      ~PlaneComparator();
  private:
      double precision{1.0e-6};
      double angularTolerance{1.0e-4 * M_PI};
      double distanceTolerance{1.0e-6};
  public:
      std::optional<bool> operator()(const GeomAdaptor_Surface& firstAdaptor,
                                     const GeomAdaptor_Surface& secondAdaptor);
      std::array<double, 4> planeCoefficients(const gp_Pln& plane) const;
      std::optional<bool> equivalentPlaneCoefficients(const gp_Pln& first, const gp_Pln& second) const;
    };
}

#endif //PLANECOMPARATOR_HPP
