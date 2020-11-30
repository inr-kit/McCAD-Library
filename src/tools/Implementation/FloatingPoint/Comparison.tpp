#ifndef COMPARISON_TPP
#define COMPARISON_TPP

#include "Comparison.hpp"

#include "DecimalDecomposition.hpp"
#include "BinaryDecomposition.hpp"

#include <cmath>
#include <algorithm>

template<typename F>
bool
McCAD::Tools::FloatingPoint::areEqual(
        const F& first,
        const F& second){
    return first == second;
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual_absolute(
        const F& first,
        const F& second,
        const F& epsilon){
    return std::abs(first - second) < epsilon;
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual_relative(
        const F& first,
        const F& second,
        const F& epsilon){
    return std::abs(first - second)
            < epsilon * std::max(std::abs(first),
                                 std::abs(second));
}

template<unsigned int significantDigits,
         typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual_decimal(
        const F& first,
        const F& second){
    return areApproxEqual<significantDigits>(
                DecimalDecomposition<F>{first},
                DecimalDecomposition<F>{second}
                );
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual_decimal(
        const F& first,
        const F& second){
    return areApproxEqual_decimal<3>(first, second);
}

template<unsigned int significantDigits,
         typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual_binary(
        const F& first,
        const F& second){
    return areApproxEqual<significantDigits>(
                BinaryDecomposition<F>{first},
                BinaryDecomposition<F>{second}
                );
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual_binary(
        const F& first,
        const F& second){
    return areApproxEqual_binary<10>(first, second);
}


template<typename F>
bool
McCAD::Tools::FloatingPoint::isZero(
        const F& number){
    return number == static_cast<F>(0);
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::isApproxZero(
        const F& number,
        const F& epsilon){
    return std::abs(number) < epsilon;
}

#endif

