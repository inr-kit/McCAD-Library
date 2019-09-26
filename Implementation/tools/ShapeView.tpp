#include "ShapeView.hpp"

#include <iterator>

template<TopAbs_ShapeEnum shapeTag>
McCAD::ShapeView<shapeTag>::ShapeView(
        const TopoDS_Shape& shape)
    : shape{shape}{
}

template<TopAbs_ShapeEnum shapeTag>
McCAD::Iterator<shapeTag>
McCAD::ShapeView<shapeTag>::begin() const{
    return shape;
}

template<TopAbs_ShapeEnum shapeTag>
McCAD::Iterator<shapeTag>
McCAD::ShapeView<shapeTag>::end() const{
    return {};
}

template<TopAbs_ShapeEnum shapeTag>
std::size_t
McCAD::ShapeView<shapeTag>::size() const{
    return std::distance(begin(), end());
}
