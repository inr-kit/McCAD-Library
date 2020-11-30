// McCAD
#include "edge.hpp"
#include "edge_impl.hpp"

McCAD::Geometry::Edge::Edge()
  : pEImpl{std::make_unique<Impl>()}{
}

McCAD::Geometry::Edge::~Edge(){
}

McCAD::Geometry::Edge::Impl*
McCAD::Geometry::Edge::accessEImpl() const{
  return pEImpl.get();
}

Standard_Boolean
McCAD::Geometry::Edge::operator == (const McCAD::Geometry::Edge& that){
  return pEImpl->isEqual(that);
}

void
McCAD::Geometry::Edge::setEdgeType(const std::string& edgeType){
  pEImpl->edgeType = edgeType;
}

std::string
McCAD::Geometry::Edge::getEdgeType(){
  return pEImpl->edgeType;
}

