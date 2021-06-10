#ifndef COORD_HASH_HPP
#define COORD_HASH_HPP

#include <functional>
#include "Coord.fwd"

namespace McCAD::Geometry{
    bool operator==(const Coord&, const Coord&);
}

namespace std{
    template<>
    struct hash<McCAD::Geometry::Coord>{
        std::size_t operator()(const McCAD::Geometry::Coord& coord) const;
    };
}

#endif
