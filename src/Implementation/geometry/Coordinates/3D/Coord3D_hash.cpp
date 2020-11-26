#include "Coord3D_hash.hpp"

#include "Coord3D.hpp"
#include "Coord_hash.hpp"

std::size_t
std::hash<McCAD::Geometry::Coord3D>::operator()(
        const McCAD::Geometry::Coord3D& vector) const{
    return std::hash<McCAD::Geometry::Coord>{}(vector.x)
            ^ std::hash<McCAD::Geometry::Coord>{}(vector.y)
            ^ std::hash<McCAD::Geometry::Coord>{}(vector.z);
}
