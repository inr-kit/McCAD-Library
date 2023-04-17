// McCAD
#include "boundSurfaceCone.hpp"
#include "boundSurfaceCone_impl.hpp"

McCAD::Geometry::BoundSurfaceCone::BoundSurfaceCone()
	: pBSCOImpl{ std::make_unique<Impl>(this) }, BoundSurface::BoundSurface() {
}

McCAD::Geometry::BoundSurfaceCone::~BoundSurfaceCone() {
}

McCAD::Geometry::BoundSurfaceCone::Impl*
McCAD::Geometry::BoundSurfaceCone::accessBSCOImpl() const {
	return pBSCOImpl.get();
}
