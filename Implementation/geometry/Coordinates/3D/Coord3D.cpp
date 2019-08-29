#include "Coord3D.hpp"

#include "Coord_const.hpp"
#include "Coord3D_util.hpp"

McCAD::Geometry::Coord3D::Coord3D()
    : x{C1D_NULL()}
    , y{C1D_NULL()}
    , z{C1D_NULL()}{
}

McCAD::Geometry::Coord3D::Coord3D(
        const Coord& x,
        const Coord& y,
        const Coord& z)
    : x{x}
    , y{y}
    , z{z}{
}

McCAD::Geometry::Coord3D&
McCAD::Geometry::Coord3D::operator+=(
        const Coord3D& that){
    this->x += that.x;
    this->y += that.y;
    this->z += that.z;
    return *this;
}

McCAD::Geometry::Coord3D&
McCAD::Geometry::Coord3D::operator-=(
        const Coord3D& that){
    this->x -= that.x;
    this->y -= that.y;
    this->z -= that.z;
    return *this;
}

McCAD::Geometry::Coord3D&
McCAD::Geometry::Coord3D::operator^=(
        const Coord3D& that){
    *this = *this ^ that;
    return *this;
}

McCAD::Geometry::Coord3D&
McCAD::Geometry::Coord3D::operator*=(
        const coord_type& alpha){
    this->x *= alpha;
    this->y *= alpha;
    this->z *= alpha;
    return *this;
}

McCAD::Geometry::Coord3D&
McCAD::Geometry::Coord3D::operator/=(
        const coord_type& alpha){
    this->x /= alpha;
    this->y /= alpha;
    this->z /= alpha;
    return *this;
}
