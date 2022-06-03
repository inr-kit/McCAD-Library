// McCAD
#include "solid.hpp"
#include "solid_impl.hpp"

McCAD::Geometry::Solid::Solid()
    : pSImpl{std::make_unique<Impl>()}{
}

McCAD::Geometry::Solid::~Solid(){
}

McCAD::Geometry::Solid::Impl*
McCAD::Geometry::Solid::accessSImpl() const{
  return pSImpl.get();
}
