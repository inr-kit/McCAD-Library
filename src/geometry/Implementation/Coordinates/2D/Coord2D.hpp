#ifndef COORD2D_HPP
#define COORD2D_HPP

#include "Coord.hpp"

namespace McCAD::Geometry{

    class Coord2D{

    public:
        Coord xi;
        Coord eta;

        Coord2D();
        Coord2D(const Coord& xi,
                const Coord& eta);

        Coord2D& operator+=(const Coord2D& that);
        Coord2D& operator-=(const Coord2D& that);
        Coord2D& operator*=(const coord_type& alpha);
        Coord2D& operator/=(const coord_type& alpha);

    };

}

#endif
