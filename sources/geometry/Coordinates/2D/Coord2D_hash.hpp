#ifndef COORD2D_HASH_HPP
#define COORD2D_HASH_HPP

#include <functional>
#include "Coord2D.fwd"

namespace McCAD::Geometry{
    bool operator==(const Coord2D&, const Coord2D&);
}

namespace std{
    template<>
    struct hash<McCAD::Geometry::Coord2D>{
        std::size_t operator()(const McCAD::Geometry::Coord2D& vector) const;
    };
}

#endif
