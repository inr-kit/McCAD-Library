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
