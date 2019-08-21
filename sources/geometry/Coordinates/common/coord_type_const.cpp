#include "coord_type_const.hpp"

#include <limits>

const McCAD::Geometry::coord_type&
McCAD::Geometry::CT_0(){
    static const auto ans = static_cast<coord_type>(0);
    return ans;
}

const McCAD::Geometry::coord_type&
McCAD::Geometry::CT_1(){
    static const auto ans = static_cast<coord_type>(1);
    return ans;
}

const McCAD::Geometry::coord_type&
McCAD::Geometry::CT_NULL(){
    static const auto ans
            = std::numeric_limits<McCAD::Geometry::coord_type>::max();
    return ans;
}

const McCAD::Geometry::coord_type&
McCAD::Geometry::CT_POS_INF(){
    static const auto ans
            = std::numeric_limits<McCAD::Geometry::coord_type>::infinity();
    return ans;
}

const McCAD::Geometry::coord_type&
McCAD::Geometry::CT_NEG_INF(){
    static const auto ans
            = -std::numeric_limits<McCAD::Geometry::coord_type>::infinity();
    return ans;
}

const McCAD::Geometry::coord_type&
McCAD::Geometry::CT_PRECISION(){
    static const auto ans = static_cast<coord_type>(0.01);
    return ans;
}
