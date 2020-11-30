#ifndef COMPARISON_HPP
#define COMPARISON_HPP

#include <limits>

namespace McCAD::Tools::FloatingPoint{

    template<typename F>
    bool areEqual(
            const F& first,
            const F& second);

    template<typename F>
    bool areApproxEqual_absolute(
            const F& first,
            const F& second,
            const F& epsilon = std::numeric_limits<F>::epsilon());

    template<typename F>
    bool areApproxEqual_relative(
            const F& first,
            const F& second,
            const F& epsilon = std::numeric_limits<F>::epsilon());

    template<unsigned int significantDigits,
             typename F>
    bool areApproxEqual_decimal(
            const F& first,
            const F& second);

    template<typename F> // significantDigits = 3
    bool areApproxEqual_decimal(
            const F& first,
            const F& second);

    template<unsigned int significantDigits,
             typename F>
    bool areApproxEqual_binary(
            const F& first,
            const F& second);

    template<typename F> // significantDigits = 10; 2^10 ~ 10^3
    bool areApproxEqual_binary(
            const F& first,
            const F& second);

    template<typename F>
    bool isZero(const F& number);

    template<typename F>
    bool isApproxZero(
            const F& number,
            const F& epsilon = std::numeric_limits<F>::epsilon());

}

#include "Comparison.tpp"

#endif
