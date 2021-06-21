#ifndef MXDSOLID_IMPL_HPP
#define MXDSOLID_IMPL_HPP

// McCAD
#include "mixedSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class MXDSolid::Impl {
  public:
    Impl() = default;

    void judgeDecomposeSurfaces(Solid::Impl* solidImpl);
    void judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl);
    void judgeThroughConcaveEdges(Solid::Impl* solidImpl);
  };
}

#endif //MXDSOLID_IMPL_HPP
