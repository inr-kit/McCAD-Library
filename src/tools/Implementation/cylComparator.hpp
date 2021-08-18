#ifndef CYLCOMPARATOR_HPP
#define CYLCOMPARATOR_HPP

// C++
#include <array>
#include <optional>
// OCC
#include <Standard.hxx>
#include <gp_Cylinder.hxx>
#include <GeomAdaptor_Surface.hxx>

namespace McCAD::Tools{
  class CylComparator{
  public:
      CylComparator();
      CylComparator(const Standard_Real& precision, const Standard_Real& angularTolerance,
                      const Standard_Real& distanceTolerance);
      ~CylComparator();
  private:
      Standard_Real precision{1.0e-7};
      Standard_Real angularTolerance{1.0e-4};
      Standard_Real distanceTolerance{1.0e-5};
  public:
      std::optional<Standard_Boolean> operator()(const GeomAdaptor_Surface& firstAdaptor,
                                                 const GeomAdaptor_Surface& secondAdaptor);
      std::array<Standard_Real, 10> cylParameters(const gp_Cylinder& cylinder) const;
      std::optional<Standard_Boolean> equivalentCylParameters(const gp_Cylinder& first,
                                                              const gp_Cylinder& second) const;
    };
}

#endif //CYLCOMPARATOR_HPP
