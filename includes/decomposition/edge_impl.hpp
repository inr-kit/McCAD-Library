#ifndef EDGE_IMPL_HPP
#define EDGE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "edge.hpp"
#include "tools_impl.hpp"
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
    McCAD::Tools::Preprocessor preproc;
    void initiate(const TopoDS_Edge& edge);
    Standard_Boolean isEqual(const McCAD::Decomposition::Edge& that);

  private:

  };
}

#endif //EDGE_IMPL_HPP
