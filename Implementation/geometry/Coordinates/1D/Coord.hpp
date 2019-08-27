#ifndef COORD_HPP
#define COORD_HPP

#include "coord_type.hpp"

namespace McCAD::Geometry{

    class Coord{

    public:
        coord_type value;

        Coord();
        Coord(const coord_type& value);

        const coord_type& operator()() const;

        Coord& operator+=(const Coord& that);
        Coord& operator-=(const Coord& that);
        Coord& operator*=(const coord_type& alpha);
        Coord& operator/=(const coord_type& alpha);

    };

}

#endif
