#ifndef BINARYDECOMPOSITION_HPP
#define BINARYDECOMPOSITION_HPP

namespace McCAD::Tools::FloatingPoint{

    template<typename F>
    class BinaryDecomposition{

    public:
        BinaryDecomposition();
        BinaryDecomposition(F value);

        const int& signum() const;
        const int& exponent() const;
        const F& mantissa() const;

        bool operator==(const BinaryDecomposition& that) const;
        bool operator!=(const BinaryDecomposition& that) const;
        explicit operator F() const;

    private:
        int signum_;
        int exponent_;
        F mantissa_;

    };

    template<unsigned int significantDigits,
             typename F>
    bool areApproxEqual(const BinaryDecomposition<F>& first,
                        const BinaryDecomposition<F>& second);

    template<typename F>
    bool areApproxEqual(const BinaryDecomposition<F>& first,
                        const BinaryDecomposition<F>& second);

}

#include "BinaryDecomposition.tpp"

#endif
