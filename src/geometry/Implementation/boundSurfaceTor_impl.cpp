// McCAD
#include "boundSurfaceTor_impl.hpp"

McCAD::Geometry::BoundSurfaceTor::Impl::Impl(
        McCAD::Geometry::BoundSurfaceTor* backReference)
    : boundSurfaceTor{backReference}{
}

McCAD::Geometry::BoundSurfaceTor::Impl::~Impl(){
}

void
McCAD::Geometry::BoundSurfaceTor::Impl::generateExtendedTor(
        const Standard_Real& boxDiagonalLength,
        Standard_Real degenerateEdgesTolerance){
}

