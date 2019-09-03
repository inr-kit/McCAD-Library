// McCAD
#include "edge_impl.hpp"

void
McCAD::Geometry::Edge::Impl::initiate(const TopoDS_Edge& aEdge){
  preproc = std::make_unique<McCAD::Tools::Preprocessor>();
  edge = aEdge;
  calculatePoints();
}

void
McCAD::Geometry::Edge::Impl::calculatePoints(){
  Standard_Real firstPoint, secondPoint, thirdPoint, fourthPoint;
  Handle_Geom_Curve curve = BRep_Tool::Curve(edge, firstPoint, secondPoint);

  curve->D0(firstPoint, startPoint);
  curve->D0(secondPoint, endPoint);

  thirdPoint = (firstPoint + secondPoint) / 2.0;
  curve->D0(thirdPoint, middlePoint);

  fourthPoint = (firstPoint + secondPoint) / 4.0;
  curve->D0(fourthPoint, extraPoint);
}

Standard_Boolean
McCAD::Geometry::Edge::Impl::isEqual(const Edge& that){
  Standard_Boolean equalityCondition = preproc->accessImpl()->isSameEdge(edge,
							   that.accessEImpl()->edge);
  return equalityCondition;
}
