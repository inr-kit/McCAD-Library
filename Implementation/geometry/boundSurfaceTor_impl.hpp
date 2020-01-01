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
    void generateExtendedTor(const Standard_Real& boxDiagonalLength,
                               Standard_Real degenerateEdgesTolerance = 1.0e-7);
  };
}

#endif //BOUNDSURFACETOR_IMPL_HPP
