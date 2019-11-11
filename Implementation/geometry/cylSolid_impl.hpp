#ifndef CYLSOLID_IMPL_HPP
#define CYLSOLID_IMPL_HPP

// C++
// McCAD
#include "cylSolid.hpp"
#include "solid_impl.hpp"
// OCC


namespace McCAD::Geometry{
  class CYLSolid::Impl {
  public:
    Impl() = default;

    std::vector<std::shared_ptr<Geometry::BoundSurface>> planesList;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> cylindersList;

    void judgeDecomposeSurfaces(Solid::Impl*& solidImpl);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl);
  };
}

#endif //CYLSOLID_IMPL_HPP
