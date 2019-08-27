#include "Coord2D.hpp"

#include "Coord_const.hpp"

McCAD::Geometry::Coord2D::Coord2D()
    : xi{C1D_NULL()}
    , eta{C1D_NULL()}{
}

McCAD::Geometry::Coord2D::Coord2D(
        const Coord& xi,
        const Coord& eta)
    : xi{xi}
    , eta{eta}{
}

McCAD::Geometry::Coord2D&
McCAD::Geometry::Coord2D::operator+=(
        const Coord2D& that){
    this->xi += that.xi;
    this->eta += that.eta;
    return *this;
}

McCAD::Geometry::Coord2D&
McCAD::Geometry::Coord2D::operator-=(
        const Coord2D& that){
    this->xi -= that.xi;
    this->eta -= that.eta;
    return *this;
}

McCAD::Geometry::Coord2D&
McCAD::Geometry::Coord2D::operator*=(
        const coord_type& alpha){
    this->xi *= alpha;
    this->eta *= alpha;
    return *this;
}

McCAD::Geometry::Coord2D&
McCAD::Geometry::Coord2D::operator/=(
        const coord_type& alpha){
    this->xi /= alpha;
    this->eta /= alpha;
    return *this;
}
