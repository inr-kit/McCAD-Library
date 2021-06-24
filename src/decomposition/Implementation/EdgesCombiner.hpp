#ifndef EDGESCOMBINER_HPP
#define EDGESCOMBINER_HPP

// C++
#include <memory>
#include <vector>
// McCAD
#include "edge_impl.hpp"

namespace McCAD::Decomposition{
  class EdgesCombiner{
  public:
      void operator()(std::vector<std::shared_ptr<Geometry::Edge>>& edgesList);
  };
}

#endif // EDGESCOMBINER_HPP
