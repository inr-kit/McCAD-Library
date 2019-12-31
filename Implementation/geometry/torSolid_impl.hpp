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

    std::vector<std::shared_ptr<Geometry::BoundSurface>> planesList;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> cylindersList;
  };
}

#endif //TORSOLID_IMPL_HPP
