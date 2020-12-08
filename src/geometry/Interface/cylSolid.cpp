// McCAD
#include "cylSolid.hpp"
#include "cylSolid_impl.hpp"

McCAD::Geometry::CYLSolid::CYLSolid()
  : pCSImpl{std::make_unique<Impl>()}, Solid::Solid(){
}

McCAD::Geometry::CYLSolid::~CYLSolid(){
}

McCAD::Geometry::CYLSolid::Impl*
McCAD::Geometry::CYLSolid::accessCSImpl() const{
  return pCSImpl.get();
}
