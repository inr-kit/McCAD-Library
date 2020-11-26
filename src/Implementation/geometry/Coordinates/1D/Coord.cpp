#include "Coord.hpp"

#include "coord_type_const.hpp"
#include "Comparison.hpp"

McCAD::Geometry::Coord::Coord()
    : value{CT_NULL()}{
}

McCAD::Geometry::Coord::Coord(
        const coord_type& value)
    : value{value}{
}

const McCAD::Geometry::coord_type&
McCAD::Geometry::Coord::operator()() const{
    return value;
}

McCAD::Geometry::Coord&
McCAD::Geometry::Coord::operator+=(
        const Coord& that){
    this->value += that.value;
    return *this;
}

McCAD::Geometry::Coord&
McCAD::Geometry::Coord::operator-=(
        const Coord& that){
    this->value -= that.value;
    return *this;
}

McCAD::Geometry::Coord&
McCAD::Geometry::Coord::operator*=(
        const coord_type& alpha){
    this->value *= alpha;
    return *this;
}

McCAD::Geometry::Coord&
McCAD::Geometry::Coord::operator/=(
        const coord_type& alpha){
    if(Tools::FloatingPoint::isZero(alpha)){
        this->value = CT_NULL();
    }else{
        this->value /= alpha;
    }
    return *this;
}
