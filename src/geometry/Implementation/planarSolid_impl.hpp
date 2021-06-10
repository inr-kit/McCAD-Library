#ifndef PLSOLID_IMPL_HPP
#define PLSOLID_IMPL_HPP

// McCAD
#include "planarSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class PLSolid::Impl{
  public:
    void judgeDecomposeSurfaces(Solid::Impl*& solidImpl);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl);
  };
}

#endif //PLSOLID_IMPL_HPP
