#ifndef BOUNDSURFACEPLANE_IMPL_HPP
#define BOUNDSURFACEPLANE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "boundSurfacePlane.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Geometry{
  class BoundSurfacePlane::Impl {
  public:
      Impl(BoundSurfacePlane* backReference);
      ~Impl();

      BoundSurfacePlane* boundSurfacePlane;
      std::string surfaceType = "Plane";
      void generateExtendedPlane(const Standard_Real& boxDiagonalLength,
                                 const Standard_Real& edgeTolerance);
  };
}

#endif //BOUNDSURFACEPLANE_IMPL_HPP
