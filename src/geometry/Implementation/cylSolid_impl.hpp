#ifndef CYLSOLID_IMPL_HPP
#define CYLSOLID_IMPL_HPP

// McCAD
#include "cylSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class CYLSolid::Impl {
  public:
    Impl() = default;

    void judgeDecomposeSurfaces(Solid::Impl* solidImpl,
                                double precision = 1.0e-6,
                                double distanceTolerance = 1.0e-6);
    void judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl,
                                      double precision = 1.0e-6,
                                      double distanceTolerance = 1.0e-6);
    void judgeThroughConcaveEdges(Solid::Impl* solidImpl);
  };
}

#endif //CYLSOLID_IMPL_HPP
