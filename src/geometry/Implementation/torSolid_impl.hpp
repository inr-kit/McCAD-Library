#ifndef TORSOLID_IMPL_HPP
#define TORSOLID_IMPL_HPP

// McCAD
#include "torSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class TORSolid::Impl {
  public:
    Impl() = default;

    void judgeDecomposeSurfaces(Solid::Impl*& solidImpl,
                                double precision = 1.0e-6,
                                double distanceTolerance = 1.0e-6);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl, 
                                  const double & distanceTolerance);
    void judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl,
                                      double precision = 1.0e-6,
                                      double distanceTolerance = 1.0e-6);
  };
}

#endif //TORSOLID_IMPL_HPP
