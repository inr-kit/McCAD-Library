// McCAD
#include "torSolid.hpp"
#include "torSolid_impl.hpp"

McCAD::Geometry::TORSolid::TORSolid()
  : pTSImpl{std::make_unique<Impl>()}, Solid::Solid(){
}

McCAD::Geometry::TORSolid::~TORSolid(){
}

McCAD::Geometry::TORSolid::Impl*
McCAD::Geometry::TORSolid::accessTSImpl() const{
  return pTSImpl.get();
}
