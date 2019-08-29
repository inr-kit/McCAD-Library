#ifndef DIMENSION_HPP
#define DIMENSION_HPP

namespace McCAD::Geometry{

    template<typename Type>
    struct dimension;

    template<typename Type>
    [[maybe_unused]] constexpr int dimension_v
        = dimension<Type>::value;

}

#endif
