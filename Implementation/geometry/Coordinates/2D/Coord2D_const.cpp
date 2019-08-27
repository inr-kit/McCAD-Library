#include "Coord2D_const.hpp"

#include "Coord2D.hpp"
#include "Coord_const.hpp"

const McCAD::Geometry::Coord2D&
McCAD::Geometry::C2D_0(){
    static Coord2D ans{
        C1D_0(),
        C1D_0()
    };
    return ans;
}

const McCAD::Geometry::Coord2D&
McCAD::Geometry::C2D_XI(){
    static Coord2D ans{
        C1D_1(),
        C1D_0()
    };
    return ans;
}

const McCAD::Geometry::Coord2D&
McCAD::Geometry::C2D_ETA(){
    static Coord2D ans{
        C1D_0(),
        C1D_1()
    };
    return ans;
}

const McCAD::Geometry::Coord2D&
McCAD::Geometry::C2D_NULL(){
    static Coord2D ans{
        C1D_NULL(),
        C1D_NULL()
    };
    return ans;
}
