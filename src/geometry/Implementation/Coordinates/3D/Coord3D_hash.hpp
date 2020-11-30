#ifndef COORD3D_HASH_HPP
#define COORD3D_HASH_HPP

#include <functional>
#include "Coord3D.fwd"

namespace McCAD::Geometry{
    bool operator==(const Coord3D&,
                    const Coord3D&);
}

namespace std{
    template<>
    struct hash<McCAD::Geometry::Coord3D>{
        std::size_t operator()(const McCAD::Geometry::Coord3D& vector) const;
    };
}

#endif
