#ifndef BOUNDSURFACEPLANE_IMPL_HPP
#define BOUNDSURFACEPLANE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "boundSurfacePlane.hpp"

namespace McCAD::Geometry{
  class BoundSurfacePlane::Impl {
  public:
      Impl(BoundSurfacePlane* backReference);
      ~Impl();

      BoundSurfacePlane* boundSurfacePlane;
      std::string surfaceType = "Plane";
      void generateExtendedPlane(const double& boxDiagonalLength,
                                 const double& edgeTolerance);
  };
}

#endif //BOUNDSURFACEPLANE_IMPL_HPP
