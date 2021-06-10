#ifndef CYLSOLID_IMPL_HPP
#define CYLSOLID_IMPL_HPP

// McCAD
#include "cylSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class CYLSolid::Impl {
  public:
    Impl() = default;

    void judgeDecomposeSurfaces(Solid::Impl* solidImpl);
    void judgeThroughConcaveEdges(Solid::Impl* solidImpl);
  };
}

#endif //CYLSOLID_IMPL_HPP
