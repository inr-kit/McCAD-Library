#ifndef TORSOLID_IMPL_HPP
#define TORSOLID_IMPL_HPP

// C++
// McCAD
#include "torSolid.hpp"
#include "solid_impl.hpp"
// OCC


namespace McCAD::Geometry{
  class TORSolid::Impl {
  public:
    Impl() = default;

    void judgeDecomposeSurfaces(Solid::Impl*& solidImpl);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl);
  };
}

#endif //TORSOLID_IMPL_HPP
