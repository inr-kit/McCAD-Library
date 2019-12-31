#ifndef BOUNDSURFACETOR_IMPL_HPP
#define BOUNDSURFACETOR_IMPL_HPP

// C++
#include <string>
// McCAD
#include "boundSurfaceTor.hpp"
#include "surface_impl.hpp"
// OCC

namespace McCAD::Geometry{
  class BoundSurfaceTor::Impl {
  public:
    Impl(BoundSurfaceTor* backReference);
    ~Impl();

    BoundSurfaceTor* boundSurfaceTor;

    std::string surfaceType = "Torus";
  };
}

#endif //BOUNDSURFACETOR_IMPL_HPP
