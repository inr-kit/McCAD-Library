#ifndef EDGE_IMPL_HPP
#define EDGE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "edge.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Edge.hxx>
#include <gp_Pnt.hxx>

namespace McCAD::Geometry{
  class Edge::Impl {
  public:
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
