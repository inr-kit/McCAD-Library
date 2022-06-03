#ifndef DECIMALDECOMPOSITION_HPP
#define DECIMALDECOMPOSITION_HPP

namespace McCAD::Tools::FloatingPoint{

    template<typename F>
    class DecimalDecomposition{

    public:
        DecimalDecomposition();
        DecimalDecomposition(F value);

        const int& signum() const;
        const int& exponent() const;
        const F& mantissa() const;

        bool operator==(const DecimalDecomposition& that) const;
        bool operator!=(const DecimalDecomposition& that) const;
        explicit operator F() const;

    private:
        int signum_;
        int exponent_;
        F mantissa_;

    };

    template<unsigned int significantDigits,
             typename F>
    bool areApproxEqual(const DecimalDecomposition<F>& first,
                        const DecimalDecomposition<F>& second);

    template<typename F>
    bool areApproxEqual(const DecimalDecomposition<F>& first,
                        const DecimalDecomposition<F>& second);

}

#include "DecimalDecomposition.tpp"

#endif
