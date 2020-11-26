#include "ShapeView.hpp"

#include <iterator>

template<TopAbs_ShapeEnum shapeTag>
McCAD::detail::ShapeView<shapeTag>::ShapeView(
        const TopoDS_Shape& shape)
    : shape{shape}{
}

template<TopAbs_ShapeEnum shapeTag>
McCAD::detail::Iterator<shapeTag>
McCAD::detail::ShapeView<shapeTag>::begin() const{
    return shape;
}

template<TopAbs_ShapeEnum shapeTag>
McCAD::detail::Iterator<shapeTag>
McCAD::detail::ShapeView<shapeTag>::end() const{
    return {};
}

template<TopAbs_ShapeEnum shapeTag>
std::size_t
McCAD::detail::ShapeView<shapeTag>::size() const{
    return std::distance(begin(), end());
}
