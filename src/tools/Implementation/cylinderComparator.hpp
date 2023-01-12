#ifndef CYLINDERCOMPARATOR_HPP
#define CYLINDERCOMPARATOR_HPP

// C++
#include <array>
#include <optional>
// OCCT
#include <GeomAdaptor_Surface.hxx>
#include <gp_Cylinder.hxx>

namespace McCAD::Tools{
  class CylinderComparator{
  public:
      CylinderComparator();
      CylinderComparator(const double& precision, const double& angularTolerance,
                         const double& distanceTolerance);
      ~CylinderComparator();
  private:
      double precision{1.0e-6};
      double angularTolerance{1.0e-4 * M_PI};
      double distanceTolerance{1.0e-6};
  public:
      std::optional<bool> operator()(const GeomAdaptor_Surface& firstAdaptor,
                                     const GeomAdaptor_Surface& secondAdaptor);
      std::array<double, 10> cylCoefficients(const gp_Cylinder& cylinder) const;
      std::optional<bool> equivalentCylCoefficients(const gp_Cylinder& first, const gp_Cylinder& second) const;
    };
}

#endif //CYLINDERCOMPARATOR_HPP
