#ifndef CONSOLID_IMPL_HPP
#define CONSOLID_IMPL_HPP

// McCAD
#include "conSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class CONSolid::Impl {
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

#endif //CONSOLID_IMPL_HPP
