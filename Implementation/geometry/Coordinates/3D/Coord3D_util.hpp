#ifndef COORD3D_UTIL_HPP
#define COORD3D_UTIL_HPP

#include "coord_type.hpp"
#include "Coord3D.fwd"
#include "Dimension.hpp"

namespace McCAD::Geometry{

    template<>
    struct dimension<Coord3D>{
        constexpr static int value = 3;
    };

    Coord3D operator+(const Coord3D& vector);
    Coord3D operator-(const Coord3D& vector);

    Coord3D operator+(const Coord3D& first,
                      const Coord3D& second);
    Coord3D operator-(const Coord3D& first,
                      const Coord3D& second);
    Coord3D operator*(const coord_type& alpha,
                      const Coord3D& vector);
    Coord3D operator*(const Coord3D& vector,
                      const coord_type& alpha);
    Coord3D operator/(const Coord3D& vector,
                      const coord_type& alpha);

    coord_type operator*(const Coord3D& first,
                         const Coord3D& second);
    Coord3D operator^(const Coord3D& first,
                      const Coord3D& second);
    coord_type dot(const Coord3D& first,
                   const Coord3D& second);
    Coord3D cross(const Coord3D& first,
                  const Coord3D& second);

    coord_type angle(const Coord3D& first,
                     const Coord3D& second);
    coord_type length(const Coord3D& vector);
    Coord3D normalize(const Coord3D& vector);

    bool operator==(const Coord3D& lhs,
                    const Coord3D& rhs);
    bool operator!=(const Coord3D& lhs,
                    const Coord3D& rhs);
    bool operator<(const Coord3D& lhs,
                   const Coord3D& rhs);
    bool operator<=(const Coord3D& lhs,
                    const Coord3D& rhs);
    bool operator>(const Coord3D& lhs,
                   const Coord3D& rhs);
    bool operator>=(const Coord3D& lhs,
                    const Coord3D& rhs);

    bool compareX(const Coord3D& lhs,
                  const Coord3D& rhs);    
    bool compareY(const Coord3D& lhs,
                  const Coord3D& rhs);
    bool compareZ(const Coord3D& lhs,
                  const Coord3D& rhs);

    bool areApproxEqual(const Coord3D& first,
                        const Coord3D& second);
    bool areApproxPerpendicular(const Coord3D& first,
                                const Coord3D& second);
    bool areApproxParallel(const Coord3D& first,
                           const Coord3D& second);
    bool areApproxAntiparallel(const Coord3D& first,
                               const Coord3D& second);
    bool isApproxUnitVector(const Coord3D& vector);

}

#endif
