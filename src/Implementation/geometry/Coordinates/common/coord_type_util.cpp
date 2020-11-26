#include "coord_type_util.hpp"

#include "coord_type_const.hpp"
#include "tools/Comparison.hpp"

bool
McCAD::Geometry::areApproxEqual(
        const coord_type& first,
        const coord_type& second){
    return Tools
            ::FloatingPoint
            ::areApproxEqual_absolute(
                first, second, CT_PRECISION());
}
