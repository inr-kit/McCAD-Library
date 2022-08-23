#ifndef BOUNDSURFACECYL_IMPL_HPP
#define BOUNDSURFACECYL_IMPL_HPP

// C++
#include <string>
// McCAD
#include "boundSurfaceCyl.hpp"
#include "surface_impl.hpp"

namespace McCAD::Geometry{
  class BoundSurfaceCyl::Impl {
  public:
    Impl(BoundSurfaceCyl* backReference);
    ~Impl();

    BoundSurfaceCyl* boundSurfaceCyl;

    std::string surfaceType = "Cylinder";
    double rangeRadian;
    void generateExtendedCyl(const double& boxDiagonalLength,
                             double tolerance = 1.0e-7);
  };
}

#endif //BOUNDSURFACECYL_IMPL_HPP
