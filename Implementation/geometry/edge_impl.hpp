#ifndef EDGE_IMPL_HPP
#define EDGE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "edge.hpp"
#include "tools_impl.hpp"
// OCC
#include <TopoDS_Edge.hxx>
#include <Geom_Curve.hxx>
#include <BRep_Tool.hxx>
#include <gp_Pnt.hxx>

namespace McCAD::Geometry{
  class Edge::Impl {
  public:
    Impl() = default;

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
