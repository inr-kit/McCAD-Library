#ifndef BINARYDECOMPOSITION_TPP
#define BINARYDECOMPOSITION_TPP

#include "BinaryDecomposition.hpp"

#include "StaticPow.hpp"
#include <cmath>

template<typename F>
McCAD::Tools::FloatingPoint::BinaryDecomposition<F>::BinaryDecomposition()
    : signum_{0}
    , exponent_{0}
    , mantissa_{static_cast<F>(0)}{
}

template<typename F>
McCAD::Tools::FloatingPoint::BinaryDecomposition<F>::BinaryDecomposition(
        F value)
    : BinaryDecomposition{}{
    if(value == static_cast<F>(0)) return;

    signum_ = (value < static_cast<F>(0))
            ? (value = -value, -1)
            : 1;

    mantissa_ = std::frexp(value, &exponent_);
    // frexp returns mantissa in [0.5, 1.0) instead of [1.0, 2.0)
    // which needs to be fixed:
    mantissa_ *= static_cast<F>(2);
    --exponent_;
}

template<typename F>
const int&
McCAD::Tools::FloatingPoint::BinaryDecomposition<F>::signum() const{
    return signum_;
}

template<typename F>
const int&
McCAD::Tools::FloatingPoint::BinaryDecomposition<F>::exponent() const{
    return exponent_;
}

template<typename F>
const F&
McCAD::Tools::FloatingPoint::BinaryDecomposition<F>::mantissa() const{
    return mantissa_;
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::BinaryDecomposition<F>::operator==(
        const BinaryDecomposition<F>& that) const{
    return this->signum_ == that.signum_
            && this->exponent_ == that.exponent_
            && this->mantissa_ == that.mantissa_;
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::BinaryDecomposition<F>::operator!=(
        const BinaryDecomposition<F>& that) const{
    return !(*this == that);
}

template<typename F>
McCAD::Tools::FloatingPoint::BinaryDecomposition<F>::operator F() const{
    return signum_ * std::ldexp(mantissa_, exponent_);
}

template<unsigned int significantDigits,
         typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual(
        const BinaryDecomposition<F>& first,
        const BinaryDecomposition<F>& second){
    if(first.signum() != second.signum()){
        return false;
    }

    auto expDiff = first.exponent() - second.exponent();
    if(expDiff > 1 || expDiff < -1){
        return false;
    }

    return std::round(
                first.mantissa()
                * pow_bin<F, significantDigits - 1>::value
                ) * std::ldexp(static_cast<F>(1), expDiff)
            == std::round(
                second.mantissa()
                * pow_bin<F, significantDigits - 1>::value
                );
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual(
        const BinaryDecomposition<F>& first,
        const BinaryDecomposition<F>& second){
    return areApproxEqual<10, F>(first, second);
}

#endif
