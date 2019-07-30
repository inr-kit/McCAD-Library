// McCAD
#include "edge_impl.hpp"

void
McCAD::Decomposition::Edge::Impl::initiate(const TopoDS_Edge& aEdge){
  edge = aEdge;
  calculatePoints();
}

Standard_Boolean
McCAD::Decomposition::Edge::Impl::isEqual(const McCAD::Decomposition::Edge& that){
  Standard_Boolean equalityCondition = preproc.accessImpl()->isSameEdge(edge, that.accessEImpl()->edge);
  return equalityCondition;
}

void
McCAD::Decomposition::Edge::Impl::calculatePoints(){
  Standard_Real firstPoint, secondPoint, thirdPoint, fourthPoint;
  Handle_Geom_Curve curve = BRep_Tool::Curve(edge, firstPoint, secondPoint);

  curve->D0(firstPoint, startPoint);
  curve->D0(secondPoint, endPoint);

  thirdPoint = (firstPoint + secondPoint) / 2.0;
  curve->D0(thirdPoint, middlePoint);

  fourthPoint = (firstPoint + secondPoint) / 4.0;
  curve->D0(fourthPoint, extraPoint);
}
