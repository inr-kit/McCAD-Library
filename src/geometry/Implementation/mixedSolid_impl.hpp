#ifndef MXDSOLID_IMPL_HPP
#define MXDSOLID_IMPL_HPP

// McCAD
#include "mixedSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class MXDSolid::Impl {
  public:
    Impl() = default;

    void judgeDecomposeSurfaces(Solid::Impl* solidImpl,
                                double precision = 1.0e-6,
                                double distanceTolerance = 1.0e-6);
    void judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl,
                                      double precision = 1.0e-6,
                                      double distanceTolerance = 1.0e-6);
    void judgeThroughConcaveEdges(Solid::Impl* solidImpl, 
                                  const double & distanceTolerance);
  };
}

#endif //MXDSOLID_IMPL_HPP
