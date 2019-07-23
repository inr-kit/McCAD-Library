#ifndef EDGE_IMPL_HPP
#define EDGE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "edge.hpp"
// OCC
#include <TopoDS_Edge.hxx>

namespace McCAD::Decomposition{
  class Edge::Impl {
  public:
    Impl() = default;

    TopoDS_Edge edge;
    std::string edgeType;
    Standard_Integer convexity;
    Standard_Boolean useForSplitSurface;
    void initiate(const TopoDS_Edge& edge);

  private:

  };
}

#endif //EDGE_IMPL_HPP
