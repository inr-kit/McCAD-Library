// McCAD
#include "edge.hpp"
#include "edge_impl.hpp"

McCAD::Decomposition::Edge::Edge()
  : pEImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::Edge::~Edge(){
}

McCAD::Decomposition::Edge::Impl*
McCAD::Decomposition::Edge::accessEImpl() const{
  return pEImpl.get();
}

void
McCAD::Decomposition::Edge::setEdgeType(const std::string& edgeType){
  pEImpl->edgeType = edgeType;
}

std::string
McCAD::Decomposition::Edge::getEdgeType(){
  return pEImpl->edgeType;
}

