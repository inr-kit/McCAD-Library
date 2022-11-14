#ifndef CONECOMPARATOR_HPP
#define CONECOMPARATOR_HPP

// C++
#include <array>
#include <optional>
// OCCT
#include <GeomAdaptor_Surface.hxx>
#include <gp_Cone.hxx>

namespace McCAD::Tools{
  class ConeComparator{
  public:
      ConeComparator();
      ConeComparator(const double& precision, const double& angularTolerance,
                     const double& distanceTolerance);
      ~ConeComparator();
  private:
      double precision{1.0e-6};
      double angularTolerance{1.0e-4 * M_PI};
      double distanceTolerance{1.0e-6};
  public:
      std::optional<bool> operator()(const GeomAdaptor_Surface& firstAdaptor,
                                     const GeomAdaptor_Surface& secondAdaptor);
      std::array<double, 10> conCoefficients(const gp_Cone& cone) const;
      std::optional<bool> equivalentConCoefficients(const gp_Cone& first, const gp_Cone& second) const;
    };
}

#endif //CONECOMPARATOR_HPP
