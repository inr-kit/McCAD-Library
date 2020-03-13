#ifndef EDGESCOMBINER_HPP
#define EDGESCOMBINER_HPP

// C++
#include <memory>
#include <vector>
// McCAD
#include <Standard.hxx>
#include "edge_impl.hpp"
// OCC

namespace McCAD::Decomposition{
  class EdgesCombiner{
  public:
      void operator()(std::vector<std::shared_ptr<McCAD::Geometry::Edge>>& edgesList);
  };
}

#endif // EDGESCOMBINER_HPP
