// McCAD
#include "edge_impl.hpp"

void
McCAD::Decomposition::Edge::Impl::initiate(const TopoDS_Edge& aEdge){
  edge = aEdge;
}
