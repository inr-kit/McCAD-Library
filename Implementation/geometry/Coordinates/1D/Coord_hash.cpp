#include "Coord_hash.hpp"

#include "Coord.hpp"
#include "coord_type.hpp"

std::size_t
std::hash<McCAD::Geometry::Coord>::operator()(
        const McCAD::Geometry::Coord& coord) const{
    return std::hash<McCAD::Geometry::coord_type>{}(coord.value);
}
