#ifndef COMMONEDGEFINDER_HPP
#define COMMONEDGEFINDER_HPP

// C++
#include <memory>
// McCAD
#include "boundSurface_impl.hpp"
#include "edge_impl.hpp"
// OCC

namespace McCAD::Decomposition{
  class CommonEdgeFinder{
  public:
      CommonEdgeFinder(const double& angularTolerance,
                       const double& distanceTolerance,
                       const double& precision);
      ~CommonEdgeFinder();
  private:
      double angularTolerance{1.0e-4 * M_PI},
             distanceTolerance{1.0e-6},
             precision{1.0e-6};
  public:
    std::vector<std::shared_ptr<McCAD::Geometry::Edge>> operator()(
            const std::shared_ptr<McCAD::Geometry::BoundSurface> firstFace,
            const std::shared_ptr<McCAD::Geometry::BoundSurface> secondFace);
  };
}

#endif // COMMONEDGEFINDER_HPP
