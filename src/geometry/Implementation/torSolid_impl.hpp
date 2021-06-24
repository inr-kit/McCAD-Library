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
                                Standard_Real precision = 1.0e-7,
                                Standard_Real distanceTolerance = 1.0e-5);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl);
    void judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl,
                                      Standard_Real precision = 1.0e-7,
                                      Standard_Real distanceTolerance = 1.0e-5);
  };
}

#endif //TORSOLID_IMPL_HPP
