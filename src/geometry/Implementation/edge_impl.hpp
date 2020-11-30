#ifndef EDGE_IMPL_HPP
#define EDGE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "edge.hpp"
#include "tools_impl.hpp"
// OCC
#include <TopoDS_Edge.hxx>

namespace McCAD::Geometry{
  class Edge::Impl {
  public:
    std::unique_ptr<Tools::Preprocessor> preproc;
    
    TopoDS_Edge edge;
    gp_Pnt startPoint, endPoint, middlePoint, extraPoint;
    std::string edgeType;
    Standard_Integer convexity;
    Standard_Boolean useForSplitSurface;

    void initiate(const TopoDS_Edge& edge);
    void calculatePoints();
    Standard_Boolean isEqual(const Edge& that);
  };
}

#endif //EDGE_IMPL_HPP
