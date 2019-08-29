#ifndef DECIMALDECOMPOSITION_TPP
#define DECIMALDECOMPOSITION_TPP

#include "DecimalDecomposition.hpp"

#include "StaticPow.hpp"
#include <cmath>

template<typename F>
McCAD::Tools::FloatingPoint::DecimalDecomposition<F>::DecimalDecomposition()
    : signum_(0)
    , exponent_(0)
    , mantissa_(static_cast<F>(0)){
}

template<typename F>
McCAD::Tools::FloatingPoint::DecimalDecomposition<F>::DecimalDecomposition(
        F value)
    : DecimalDecomposition(){
    if(value == static_cast<F>(0)) return;

    signum_ = (value < static_cast<F>(0))
            ? (value = -value, -1)
            : 1;

    exponent_ = static_cast<int>(std::floor(std::log10(value)));
    mantissa_ = value * std::pow(static_cast<F>(10), -exponent_);
}

template<typename F>
const int&
McCAD::Tools::FloatingPoint::DecimalDecomposition<F>::signum() const{
    return signum_;
}

template<typename F>
const int&
McCAD::Tools::FloatingPoint::DecimalDecomposition<F>::exponent() const{
    return exponent_;
}

template<typename F>
const F&
McCAD::Tools::FloatingPoint::DecimalDecomposition<F>::mantissa() const{
    return mantissa_;
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::DecimalDecomposition<F>::operator==(
        const DecimalDecomposition<F>& that) const{
    return this->signum_ == that.signum_
            && this->exponent_ == that.exponent_
            && this->mantissa_ == that.mantissa_;
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::DecimalDecomposition<F>::operator!=(
        const DecimalDecomposition<F>& that) const{
    return !(*this == that);
}

template<typename F>
McCAD::Tools::FloatingPoint::DecimalDecomposition<F>::operator F() const{
    return signum_ * mantissa_ * std::pow(static_cast<F>(10), exponent_);
}

template<unsigned int significantDigits,
         typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual(
        const DecimalDecomposition<F>& first,
        const DecimalDecomposition<F>& second){

    if(first.signum() != second.signum()){
        return false;
    }

    auto expDiff = first.exponent() - second.exponent();
    if(expDiff > 1 || expDiff < -1){
        return false;
    }

    return std::round(
                first.mantissa()
                * pow_dec<F, significantDigits - 1>::value
                ) * std::pow(static_cast<F>(10), expDiff)
            == std::round(
                second.mantissa()
                * pow_dec<F, significantDigits - 1>::value
                );
}

template<typename F>
bool
McCAD::Tools::FloatingPoint::areApproxEqual(
        const DecimalDecomposition<F>& first,
        const DecimalDecomposition<F>& second){
    return areApproxEqual<3, F>(first, second);
}

#endif

