// McCAD
#include "meshtriangle.hpp"
#include "meshtriangle_impl.hpp"

McCAD::Geometry::MeshTriangle::MeshTriangle()
  : pMTImpl{std::make_unique<Impl>()}{
}

McCAD::Geometry::MeshTriangle::~MeshTriangle(){
}

McCAD::Geometry::MeshTriangle::Impl*
McCAD::Geometry::MeshTriangle::accessMTImpl() const{
  return pMTImpl.get();
}
