// McCAD
#include "meshtriangle.hpp"
#include "meshtriangle_impl.hpp"

McCAD::Decomposition::MeshTriangle::MeshTriangle()
  : pMTImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::MeshTriangle::~MeshTriangle(){
}

McCAD::Decomposition::MeshTriangle::Impl*
McCAD::Decomposition::MeshTriangle::accessMTImpl() const{
  return pMTImpl.get();
}
