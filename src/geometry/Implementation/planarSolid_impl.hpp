#ifndef PLSOLID_IMPL_HPP
#define PLSOLID_IMPL_HPP

// McCAD
#include "planarSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class PLSolid::Impl{
  public:
    void judgeDecomposeSurfaces(Solid::Impl*& solidImpl,
                                double precision = 1.0e-6,
                                double distanceTolerance = 1.0e-6);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl, 
                                  const double& distanceTolerance);
  };
}

#endif //PLSOLID_IMPL_HPP
