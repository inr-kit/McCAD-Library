#include "Coord2D_hash.hpp"
#include "Coord2D.hpp"
#include "Coord_hash.hpp"

std::size_t std::hash<McCAD::Geometry::Coord2D>::operator()(const McCAD::Geometry::Coord2D& vector) const{
    return std::hash<McCAD::Geometry::Coord>()(vector.xi)
            ^ std::hash<McCAD::Geometry::Coord>()(vector.eta);
}
