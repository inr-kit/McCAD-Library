#ifndef STATICPOW_HPP
#define STATICPOW_HPP

#include <type_traits>
#include <cstddef>

namespace McCAD::Tools::FloatingPoint{

    template<typename F,
             unsigned int base,
             int exponent,
             typename Enable = void>
    struct pow;

    template<typename F,
             unsigned int base,
             int exponent>
    struct pow<F, base, exponent, typename std::enable_if_t<(exponent == 0)>>{
        static constexpr F value
            = static_cast<F>(1);
    };

    template<typename F,
             unsigned int base,
             int exponent>
    struct pow<F, base, exponent, typename std::enable_if_t<(exponent < 0)>>{
        static constexpr F value
            = static_cast<F>(1) / pow<F, base, -exponent>::value;
    };

    template<typename F,
             unsigned int base,
             int exponent>
    struct pow<F, base, exponent,
            typename std::enable_if_t<(exponent > 0 && exponent % 2 == 0)>>{
        static constexpr F value
            = pow<F, base, exponent / 2>::value
            * pow<F, base, exponent / 2>::value;
    };

    template<typename F,
             unsigned int base,
             int exponent>
    struct pow<F, base, exponent,
            typename std::enable_if_t<(exponent > 0 && exponent % 2 != 0)>>{
        static constexpr F value
            = static_cast<F>(base)
            * pow<F, base, (exponent - 1) / 2>::value
            * pow<F, base, (exponent - 1) / 2>::value;
    };


    template<typename F,
             int exponent>
    using pow_dec = pow<F, 10, exponent>;

    template<typename F,
             int exponent>
    using pow_bin = pow<F, 2, exponent>;

}

#endif
