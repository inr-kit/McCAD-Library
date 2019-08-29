#ifndef COORD2D_UTIL_HPP
#define COORD2D_UTIL_HPP

#include "coord_type.hpp"
#include "Coord2D.fwd"
#include "Dimension.hpp"

namespace McCAD::Geometry{

    template<>
    struct dimension<Coord2D>{
        constexpr static int value = 2;
    };

    Coord2D operator+(const Coord2D& vector);
    Coord2D operator-(const Coord2D& vector);

    Coord2D operator+(const Coord2D& first,
                      const Coord2D& second);
    Coord2D operator-(const Coord2D& first,
                      const Coord2D& second);
    Coord2D operator*(const coord_type& alpha,
                      const Coord2D& vector);
    Coord2D operator*(const Coord2D& vector,
                      const coord_type& alpha);
    Coord2D operator/(const Coord2D& vector,
                      const coord_type& alpha);

    coord_type operator*(const Coord2D& first,
                         const Coord2D& second);
    coord_type dot(const Coord2D& first,
                   const Coord2D& second);

    coord_type angle(const Coord2D& first,
                     const Coord2D& second);
    coord_type length(const Coord2D& vector);
    Coord2D normalize(const Coord2D& vector);

    bool operator==(const Coord2D& lhs,
                    const Coord2D& rhs);
    bool operator!=(const Coord2D& lhs,
                    const Coord2D& rhs);
    bool operator<(const Coord2D& lhs,
                   const Coord2D& rhs);
    bool operator<=(const Coord2D& lhs,
                    const Coord2D& rhs);
    bool operator>(const Coord2D& lhs,
                   const Coord2D& rhs);
    bool operator>=(const Coord2D& lhs,
                    const Coord2D& rhs);

    bool compareXi(const Coord2D& lhs,
                   const Coord2D& rhs);
    bool compareEta(const Coord2D& lhs,
                    const Coord2D& rhs);

    bool areApproxEqual(const Coord2D& lhs,
                        const Coord2D& rhs);
    bool areApproxPerpendicular(const Coord2D& lhs,
                                const Coord2D& rhs);
    bool areApproxParallel(const Coord2D& lhs,
                           const Coord2D& rhs);
    bool areApproxAntiparallel(const Coord2D& lhs,
                               const Coord2D& rhs);
    bool isApproxUnitVector(const Coord2D& vector);

}

#endif
