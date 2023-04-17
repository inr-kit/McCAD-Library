// McCAD
#include "coneSolid.hpp"
#include "coneSolid_impl.hpp"

McCAD::Geometry::CONESolid::CONESolid()
	: pCOSImpl{ std::make_unique<Impl>() }, Solid::Solid() {
}

McCAD::Geometry::CONESolid::~CONESolid() {
}

McCAD::Geometry::CONESolid::Impl*
McCAD::Geometry::CONESolid::accessCOSImpl() const {
	return pCOSImpl.get();
}
