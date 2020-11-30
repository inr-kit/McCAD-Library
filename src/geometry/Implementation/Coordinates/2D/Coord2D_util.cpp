#include "Coord2D_util.hpp"
#include "Coord2D.hpp"
#include "Coord2D_const.hpp"
#include "Coord_util.hpp"
#include "Coord_const.hpp"
#include "coord_type_const.hpp"
#include "Comparison.hpp"
#include <cmath>

McCAD::Geometry::Coord2D
McCAD::Geometry::operator+(
        const Coord2D& vector){
    return vector;
}

McCAD::Geometry::Coord2D
McCAD::Geometry::operator-(
        const Coord2D& vector){
    return {
        -vector.xi,
        -vector.eta
    };
}

McCAD::Geometry::Coord2D
McCAD::Geometry::operator+(
        const Coord2D& first,
        const Coord2D& second){
    auto temp{first};
    temp += second;
    return temp;
}

McCAD::Geometry::Coord2D
McCAD::Geometry::operator-(
        const Coord2D& first,
        const Coord2D& second){
    auto temp{first};
    temp -= second;
    return temp;
}

McCAD::Geometry::Coord2D
McCAD::Geometry::operator*(
        const coord_type& alpha,
        const Coord2D& vector){
    auto temp{vector};
    temp *= alpha;
    return temp;
}

McCAD::Geometry::Coord2D
McCAD::Geometry::operator*(
        const Coord2D& vector,
        const coord_type& alpha){
    auto temp{vector};
    temp *= alpha;
    return temp;
}

McCAD::Geometry::Coord2D
McCAD::Geometry::operator/(
        const Coord2D& vector,
        const coord_type& alpha){
    auto temp{vector};
    temp /= alpha;
    return temp;
}

McCAD::Geometry::coord_type
McCAD::Geometry::operator*(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return lhs.xi() * rhs.xi()
            + lhs.eta() * rhs.eta();
}

McCAD::Geometry::coord_type
McCAD::Geometry::dot(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return lhs * rhs;
}

McCAD::Geometry::coord_type
McCAD::Geometry::angle(
        const Coord2D& lhs,
        const Coord2D& rhs){
    if(lhs == C2D_0()
            || rhs == C2D_0()
            || lhs == C2D_NULL()
            || rhs == C2D_NULL()){
        return CT_NULL();
    }else{
        return std::acos(dot(lhs, rhs) / (length(lhs) * length(rhs)));
    }
}

McCAD::Geometry::coord_type
McCAD::Geometry::length(
        const Coord2D& vector){
    if(vector == C2D_NULL()){
        return CT_NULL();
    }else{
        return hypot(vector.xi(), vector.eta());
    }
}

McCAD::Geometry::Coord2D
McCAD::Geometry::normalize(
        const Coord2D& vector){
    if(vector == McCAD::Geometry::C2D_0()
            || vector == McCAD::Geometry::C2D_NULL()){
        return C2D_NULL();
    }else{
        return vector / length(vector);
    }
}

bool
McCAD::Geometry::operator==(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return lhs.xi == rhs.xi
            && lhs.eta == rhs.eta;
}

bool
McCAD::Geometry::operator!=(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return !(lhs == rhs);
}

bool
McCAD::Geometry::operator<(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return lhs.xi < rhs.xi
            || (lhs.xi == rhs.xi && lhs.eta < rhs.eta);
}

bool
McCAD::Geometry::operator<=(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return lhs == rhs || lhs < rhs;
}

bool
McCAD::Geometry::operator>(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return rhs < lhs;
}

bool
McCAD::Geometry::operator>=(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return rhs == lhs || rhs < lhs;
}

bool
McCAD::Geometry::compareXi(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return lhs.xi < rhs.xi;
}

bool
McCAD::Geometry::compareEta(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return lhs.eta < rhs.eta;
}

bool
McCAD::Geometry::areApproxEqual(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return areApproxEqual(lhs.xi, rhs.xi)
            && areApproxEqual(lhs.eta, rhs.eta);
}

bool
McCAD::Geometry::areApproxPerpendicular(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return Tools
            ::FloatingPoint
            ::isApproxZero(
                dot(lhs, rhs),
                CT_PRECISION()
                );
}

bool
McCAD::Geometry::areApproxParallel(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return areApproxEqual(dot(lhs, rhs), length(lhs) * length(rhs));
}

bool
McCAD::Geometry::areApproxAntiparallel(
        const Coord2D& lhs,
        const Coord2D& rhs){
    return areApproxParallel(lhs, -rhs);
}

bool
McCAD::Geometry::isApproxUnitVector(
        const Coord2D& vector){
    return areApproxEqual(length(vector), C1D_1());
}
