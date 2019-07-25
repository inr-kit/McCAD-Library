// McCAD
#include "edge_impl.hpp"

void
McCAD::Decomposition::Edge::Impl::initiate(const TopoDS_Edge& aEdge){
  edge = aEdge;
}

Standard_Boolean
McCAD::Decomposition::Edge::Impl::isEqual(const McCAD::Decomposition::Edge& that){
  Standard_Boolean equalityCondition = preproc.accessImpl()->isSameEdge(edge, that.accessEImpl()->edge);
  return equalityCondition;
}

