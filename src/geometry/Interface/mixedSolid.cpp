// McCAD
#include "mixedSolid.hpp"
#include "mixedSolid_impl.hpp"

McCAD::Geometry::MXDSolid::MXDSolid()
  : pXSImpl{std::make_unique<Impl>()}, Solid::Solid(){
}

McCAD::Geometry::MXDSolid::~MXDSolid(){
}

McCAD::Geometry::MXDSolid::Impl*
McCAD::Geometry::MXDSolid::accessXSImpl() const{
  return pXSImpl.get();
}
