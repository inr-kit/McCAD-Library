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
      CommonEdgeFinder(const Standard_Real& angularTolerance,
                       const Standard_Real& distanceTolerance,
                       const Standard_Real& precision);
      ~CommonEdgeFinder();
  private:
      Standard_Real angularTolerance{1.0e-4 * M_PI},
                    distanceTolerance{1.0e-6},
                    precision{1.0e-6};
  public:
    std::vector<std::shared_ptr<McCAD::Geometry::Edge>> operator()(
            const std::shared_ptr<McCAD::Geometry::BoundSurface> firstFace,
            const std::shared_ptr<McCAD::Geometry::BoundSurface> secondFace);
  };
}

#endif // COMMONEDGEFINDER_HPP
