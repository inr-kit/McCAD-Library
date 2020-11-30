#ifndef COORD3D_HPP_INCLUDED
#define COORD3D_HPP_INCLUDED

#include "Coord.hpp"

namespace McCAD::Geometry{

    class Coord3D{

    public:
        Coord x;
        Coord y;
        Coord z;

        Coord3D();
        Coord3D(const Coord& x,
                const Coord& y,
                const Coord& z);

        Coord3D& operator+=(const Coord3D& that);
        Coord3D& operator-=(const Coord3D& that);
        Coord3D& operator^=(const Coord3D& that);
        Coord3D& operator*=(const coord_type& alpha);
        Coord3D& operator/=(const coord_type& alpha);

    };

}

#endif
