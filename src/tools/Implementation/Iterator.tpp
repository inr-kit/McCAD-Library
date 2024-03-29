#include "Iterator.hpp"

template<TopAbs_ShapeEnum shapeTag>
McCAD::detail::Iterator<shapeTag>::Iterator(
        const TopoDS_Shape& shape)
    : explorer{std::make_shared<TopExp_Explorer>(shape, shapeTag)}{
    if(!explorer->More())
        explorer = nullptr;
}

template<TopAbs_ShapeEnum shapeTag>
bool
McCAD::detail::Iterator<shapeTag>::operator==(
        const Iterator& that) const{
    return this->explorer == that.explorer;
}

template<TopAbs_ShapeEnum shapeTag>
bool
McCAD::detail::Iterator<shapeTag>::operator!=(
        const Iterator& that) const{
    return !(*this == that);
}

template<TopAbs_ShapeEnum shapeTag>
McCAD::detail::Iterator<shapeTag>&
McCAD::detail::Iterator<shapeTag>::operator++(){
    explorer->Next();
    if(!explorer->More())
        explorer = nullptr;
    return *this;
}

template<TopAbs_ShapeEnum shapeTag>
McCAD::detail::Iterator<shapeTag>
McCAD::detail::Iterator<shapeTag>::operator++(int){
    auto copy{*this};
    ++*this;
    return copy;
}

template<TopAbs_ShapeEnum shapeTag>
const typename McCAD::detail::TopClass<shapeTag>::type&
McCAD::detail::Iterator<shapeTag>::operator*(){
    return (*detail::TopClass<shapeTag>::castFunction)(explorer->Current());
}
