#include "Coord_const.hpp"

#include "Coord.hpp"
#include "coord_type_const.hpp"

const McCAD::Geometry::Coord&
McCAD::Geometry::C1D_0(){
    static const Coord ans{CT_0()};
    return ans;
}

const McCAD::Geometry::Coord&
McCAD::Geometry::C1D_1(){
    static const Coord ans{CT_1()};
    return ans;
}

const McCAD::Geometry::Coord&
McCAD::Geometry::C1D_NULL(){
    static const Coord ans{CT_NULL()};
    return ans;
}

const McCAD::Geometry::Coord&
McCAD::Geometry::C1D_POS_INF(){
    static const Coord ans{CT_POS_INF()};
    return ans;
}

const McCAD::Geometry::Coord&
McCAD::Geometry::C1D_NEG_INF(){
    static const Coord ans{CT_NEG_INF()};
    return ans;
}
