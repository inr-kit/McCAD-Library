#ifndef PLSOLID_IMPL_HPP
#define PLSOLID_IMPL_HPP

// C++
#include<memory>
// McCAD
#include "planarSolid.hpp"
#include "solid_impl.hpp"
// OCC

namespace McCAD::Geometry{
  class PLSolid::Impl{
  public:
    Impl() = default;

    std::vector<std::shared_ptr<Geometry::BoundSurface>> planesList;

    void judgeDecomposeSurfaces(Solid::Impl*& solidImpl);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl);
  };
}

#endif //PLSOLID_IMPL_HPP
