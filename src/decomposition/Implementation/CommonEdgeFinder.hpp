#ifndef COMMONEDGEFINDER_HPP
#define COMMONEDGEFINDER_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
#include "boundSurface_impl.hpp"
#include "edge_impl.hpp"
// OCC

namespace McCAD::Decomposition{
  class CommonEdgeFinder{
  public:
    std::vector<std::shared_ptr<McCAD::Geometry::Edge>> operator()(
            const std::shared_ptr<McCAD::Geometry::BoundSurface> firstFace,
            const std::shared_ptr<McCAD::Geometry::BoundSurface> secondFace);
  };
}

#endif // COMMONEDGEFINDER_HPP
