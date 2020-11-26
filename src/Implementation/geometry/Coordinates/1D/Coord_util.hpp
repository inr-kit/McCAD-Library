#ifndef COORD_UTIL_HPP
#define COORD_UTIL_HPP

#include "coord_type.hpp"
#include "Coord.fwd"
#include "Dimension.hpp"

namespace McCAD::Geometry{

    template<>
    struct dimension<Coord>{
        constexpr static int value = 1;
    };

    Coord operator+(const Coord& coord);
    Coord operator-(const Coord& coord);

    Coord operator+(const Coord& first,
                    const Coord& second);
    Coord operator-(const Coord& first,
                    const Coord& second);
    Coord operator*(const coord_type& alpha,
                    const Coord& coord);
    Coord operator*(const Coord& coord,
                    const coord_type& alpha);
    Coord operator/(const Coord& coord,
                    const coord_type& alpha);

    coord_type operator*(const Coord& first,
                         const Coord& second);
    coord_type operator/(const Coord& first,
                         const Coord& second);
    coord_type dot(const Coord& first,
                   const Coord& second);
    coord_type ratio(const Coord& first,
                     const Coord& second);

    coord_type length(const Coord& coord);

    bool operator==(const Coord& lhs,
                    const Coord& rhs);
    bool operator!=(const Coord& lhs,
                    const Coord& rhs);
    bool operator<(const Coord& lhs,
                   const Coord& rhs);
    bool operator<=(const Coord& lhs,
                    const Coord& rhs);
    bool operator>(const Coord& lhs,
                   const Coord& rhs);
    bool operator>=(const Coord& lhs,
                    const Coord& rhs);
    bool areApproxEqual(const Coord& first,
                        const Coord& second);

}

#endif
