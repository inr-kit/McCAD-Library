#include "Coord3D_util.hpp"

#include "Coord3D.hpp"
#include "Coord3D_const.hpp"
#include "Coord_util.hpp"
#include "coord_type_const.hpp"
#include "Coord_const.hpp"
#include "Comparison.hpp"
#include <cmath>

McCAD::Geometry::Coord3D
McCAD::Geometry::operator+(
        const Coord3D& vector){
    return vector;
}

McCAD::Geometry::Coord3D
McCAD::Geometry::operator-(
        const Coord3D& vector){
    return {
        -vector.x,
        -vector.y,
        -vector.z
    };
}

McCAD::Geometry::Coord3D
McCAD::Geometry::operator+(
        const Coord3D& first,
        const Coord3D& second){
    auto temp{first};
    temp += second;
    return temp;
}

McCAD::Geometry::Coord3D
McCAD::Geometry::operator-(
        const Coord3D& first,
        const Coord3D& second){
    auto temp{first};
    temp -= second;
    return temp;
}

McCAD::Geometry::Coord3D
McCAD::Geometry::operator*(
        const coord_type& alpha,
        const Coord3D& vector){
    auto temp{vector};
    temp *= alpha;
    return temp;
}

McCAD::Geometry::Coord3D
McCAD::Geometry::operator*(
        const Coord3D& vector,
        const coord_type& alpha){
    auto temp{vector};
    temp *= alpha;
    return temp;
}

McCAD::Geometry::Coord3D
McCAD::Geometry::operator/(
        const Coord3D& vector,
        const coord_type& alpha){
    auto temp{vector};
    temp /= alpha;
    return temp;
}

McCAD::Geometry::coord_type
McCAD::Geometry::operator*(
        const Coord3D& first,
        const Coord3D& second){
    return first.x() * second.x()
            + first.y() * second.y()
            + first.z() * second.z();
}

McCAD::Geometry::Coord3D
McCAD::Geometry::operator^(
        const Coord3D& first,
        const Coord3D& second){
    return {
        first.y() * second.z() - first.z() * second.y(),
        first.z() * second.x() - first.x() * second.z(),
        first.x() * second.y() - first.y() * second.x()
    };
}

McCAD::Geometry::coord_type
McCAD::Geometry::dot(
        const Coord3D& first,
        const Coord3D& second){
    return first * second;
}

McCAD::Geometry::Coord3D
McCAD::Geometry::cross(
        const Coord3D& first,
        const Coord3D& second){
    return first ^ second;
}

McCAD::Geometry::coord_type
McCAD::Geometry::angle(
        const Coord3D& lhs,
        const Coord3D& rhs){
    if(lhs == C3D_0()
            || rhs == C3D_0()
            || lhs == C3D_NULL()
            || rhs == C3D_NULL()){
        return CT_NULL();
    }else{
        return std::acos(dot(lhs, rhs) / (length(lhs) * length(rhs)));
    }
}

McCAD::Geometry::coord_type
McCAD::Geometry::length(
        const Coord3D& vector){
    if(vector == C3D_NULL()){
        return CT_NULL();
    }else{
        return hypot(vector.x(),
                     hypot(vector.y(),
                           vector.z()));
    }
}

McCAD::Geometry::Coord3D
McCAD::Geometry::normalize(
        const Coord3D& vector){
    if(vector == McCAD::Geometry::C3D_0()
            || vector == McCAD::Geometry::C3D_NULL()){
        return C3D_NULL();
    }else{
        return vector / length(vector);
    }
}

bool
McCAD::Geometry::operator==(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return lhs.x == rhs.x
            && lhs.y == rhs.y
            && lhs.z == rhs.z;
}

bool
McCAD::Geometry::operator!=(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return !(lhs == rhs);
}

bool
McCAD::Geometry::operator<(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return lhs.x < rhs.x
            || (lhs.x == rhs.x && lhs.y < rhs.y)
            || (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z < rhs.z);
}

bool
McCAD::Geometry::operator<=(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return lhs == rhs || lhs < rhs;
}

bool
McCAD::Geometry::operator>(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return rhs < lhs;
}

bool
McCAD::Geometry::operator>=(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return rhs == lhs || rhs < lhs;
}

bool
McCAD::Geometry::compareX(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return lhs.x < rhs.x;
}

bool
McCAD::Geometry::compareY(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return lhs.y < rhs.y;
}

bool
McCAD::Geometry::compareZ(
        const Coord3D& lhs,
        const Coord3D& rhs){
    return lhs.z < rhs.z;
}

bool
McCAD::Geometry::areApproxEqual(const Coord3D& first,
        const Coord3D& second){
    return areApproxEqual(first.x, second.x)
            && areApproxEqual(first.y, second.y)
            && areApproxEqual(first.z, second.z);
}

bool
McCAD::Geometry::areApproxPerpendicular(
        const Coord3D& first,
        const Coord3D& second){
    return Tools
            ::FloatingPoint
            ::isApproxZero(
                dot(first, second),
                CT_PRECISION()
                );
}

bool
McCAD::Geometry::areApproxParallel(const Coord3D& first,
        const Coord3D& second){
    return areApproxEqual(dot(first, second), length(first) * length(second));
}

bool
McCAD::Geometry::areApproxAntiparallel(const Coord3D& first,
        const Coord3D& second){
    return areApproxParallel(first, -second);
}

bool
McCAD::Geometry::isApproxUnitVector(
        const Coord3D& vector){
    return areApproxEqual(length(vector), C1D_1());
}