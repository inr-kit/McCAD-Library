#include "Coord_util.hpp"

#include "Coord.hpp"
#include "Coord_const.hpp"
#include "coord_type_const.hpp"
#include "coord_type_util.hpp"
#include <cmath>

McCAD::Geometry::Coord
McCAD::Geometry::operator+(
        const Coord& coord){
    return coord;
}

McCAD::Geometry::Coord
McCAD::Geometry::operator-(
        const Coord& coord){
    return -coord.value;
}

McCAD::Geometry::Coord
McCAD::Geometry::operator+(
        const Coord& first,
        const Coord& second){
    auto temp{first};
    temp += second;
    return temp;
}

McCAD::Geometry::Coord
McCAD::Geometry::operator-(
        const Coord& first,
        const Coord& second){
    auto temp{first};
    temp -= second;
    return temp;
}

McCAD::Geometry::Coord
McCAD::Geometry::operator*(
        const coord_type& alpha,
        const Coord& coord){
    auto temp{coord};
    temp *= alpha;
    return temp;
}

McCAD::Geometry::Coord
McCAD::Geometry::operator*(
        const Coord& coord,
        const coord_type& alpha){
    auto temp{coord};
    temp *= alpha;
    return temp;
}

McCAD::Geometry::Coord
McCAD::Geometry::operator/(
        const Coord& coord,
        const coord_type& alpha){
    auto temp{coord};
    temp /= alpha;
    return temp;
}

McCAD::Geometry::coord_type
McCAD::Geometry::operator*(
        const Coord& first,
        const Coord& second){
    return first.value * second.value;
}

McCAD::Geometry::coord_type
McCAD::Geometry::operator/(
        const Coord& first,
        const Coord& second){
    return first.value / second.value;
}

McCAD::Geometry::coord_type
McCAD::Geometry::dot(
        const Coord& first,
        const Coord& second){
    return first * second;
}

McCAD::Geometry::coord_type
McCAD::Geometry::ratio(
        const Coord& first,
        const Coord& second){
    return first / second;
}

McCAD::Geometry::coord_type
McCAD::Geometry::length(
        const Coord& coord){
    return std::abs(coord.value);
}

bool
McCAD::Geometry::operator==(
        const Coord& lhs,
        const Coord& rhs){
    return lhs.value == rhs.value;
}

bool
McCAD::Geometry::operator!=(
        const Coord& lhs,
        const Coord& rhs){
    return !(lhs == rhs);
}

bool
McCAD::Geometry::operator<(
        const Coord& lhs,
        const Coord& rhs){
    return lhs != C1D_NULL()
            && rhs != C1D_NULL()
            && lhs.value < rhs.value;
}

bool
McCAD::Geometry::operator<=(
        const Coord& lhs,
        const Coord& rhs){
    return lhs == rhs || lhs < rhs;
}

bool
McCAD::Geometry::operator>(
        const Coord& lhs,
        const Coord& rhs){
    return rhs < lhs;
}

bool
McCAD::Geometry::operator>=(
        const Coord& lhs,
        const Coord& rhs){
    return rhs == lhs || rhs < lhs;
}

bool
McCAD::Geometry::areApproxEqual(
        const Coord& first,
        const Coord& second){
    return areApproxEqual(first.value, second.value);
}