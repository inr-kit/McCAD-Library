// McCAD
#include "planarSolid.hpp"
#include "planarSolid_impl.hpp"

McCAD::Geometry::PLSolid::PLSolid()
  : pPSImpl{std::make_unique<Impl>()}, Solid::Solid(){
}

McCAD::Geometry::PLSolid::~PLSolid(){
}

McCAD::Geometry::PLSolid::Impl*
McCAD::Geometry::PLSolid::accessPSImpl() const{
  return pPSImpl.get();
}
