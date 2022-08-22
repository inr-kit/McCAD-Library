// McCAD
#include "conSolid.hpp"
#include "conSolid_impl.hpp"

McCAD::Geometry::CONSolid::CONSolid()
  : pCONSImpl{std::make_unique<Impl>()}, Solid::Solid(){
}

McCAD::Geometry::CONSolid::~CONSolid(){
}

McCAD::Geometry::CONSolid::Impl*
McCAD::Geometry::CONSolid::accessCONSImpl() const{
  return pCONSImpl.get();
}
