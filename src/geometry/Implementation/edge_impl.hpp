#ifndef EDGE_IMPL_HPP
#define EDGE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "edge.hpp"
// OCCT
#include <TopoDS_Edge.hxx>
#include <gp_Pnt.hxx>

namespace McCAD::Geometry{
  class Edge::Impl {
  public:
    TopoDS_Edge edge;
    gp_Pnt startPoint, endPoint, middlePoint, extraPoint;
    std::string edgeType;
    int convexity;
    bool useForSplitSurface;

    void initiate(const TopoDS_Edge& edge);
    void calculatePoints();
    bool isEqual(const Edge& that);
  };
}

#endif //EDGE_IMPL_HPP
