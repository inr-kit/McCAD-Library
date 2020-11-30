#include "Coord3D_const.hpp"

#include "Coord3D.hpp"
#include "Coord_const.hpp"

const McCAD::Geometry::Coord3D&
McCAD::Geometry::C3D_0(){
    static Coord3D ans{
        C1D_0(),
        C1D_0(),
        C1D_0()
    };
    return ans;
}

const McCAD::Geometry::Coord3D&
McCAD::Geometry::C3D_X(){
    static Coord3D ans{
        C1D_1(),
        C1D_0(),
        C1D_0()
    };
    return ans;
}

const McCAD::Geometry::Coord3D&
McCAD::Geometry::C3D_Y(){
    static Coord3D ans{
        C1D_0(),
        C1D_1(),
        C1D_0()
    };
    return ans;
}

const McCAD::Geometry::Coord3D&
McCAD::Geometry::C3D_Z(){
    static Coord3D ans{
        C1D_0(),
        C1D_0(),
        C1D_1()
    };
    return ans;
}

const McCAD::Geometry::Coord3D&
McCAD::Geometry::C3D_NULL(){
    static Coord3D ans{
        C1D_NULL(),
        C1D_NULL(),
        C1D_NULL()
    };
    return ans;
}
