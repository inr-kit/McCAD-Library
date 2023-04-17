// C++
#include <array>
// McCAD
#include "boundSurfaceCone_impl.hpp"
// OCC
#include <Geom_Surface.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRep_Tool.hxx>

McCAD::Geometry::BoundSurfaceCone::Impl::Impl(McCAD::Geometry::BoundSurfaceCone* backReference)
    : boundSurfaceCone{ backReference } {
}

McCAD::Geometry::BoundSurfaceCone::Impl::~Impl() {
}

void
McCAD::Geometry::BoundSurfaceCone::Impl::generateExtendedCone(
    const Standard_Real& boxDiagonalLength,  
    Standard_Real degenerateEdgesTolerance) {
}
