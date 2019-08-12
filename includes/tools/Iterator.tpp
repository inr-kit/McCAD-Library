#include "Iterator.hpp"

template<TopAbs_ShapeEnum shapeTag>
McCAD::Iterator<shapeTag>::Iterator(
        const TopoDS_Shape& shape)
    : explorer{std::make_shared<TopExp_Explorer>(shape, shapeTag)}{
}

template<TopAbs_ShapeEnum shapeTag>
bool
McCAD::Iterator<shapeTag>::operator==(
        const Iterator& that) const{
    return this->explorer == that.explorer;
}

template<TopAbs_ShapeEnum shapeTag>
bool
McCAD::Iterator<shapeTag>::operator!=(
        const Iterator& that) const{
    return !(*this == that);
}

template<TopAbs_ShapeEnum shapeTag>
McCAD::Iterator<shapeTag>&
McCAD::Iterator<shapeTag>::operator++(){
    explorer->Next();
    if(!explorer->More())
        explorer = nullptr;
    return *this;
}

template<TopAbs_ShapeEnum shapeTag>
McCAD::Iterator<shapeTag>
McCAD::Iterator<shapeTag>::operator++(int){
    auto copy{*this};
    ++*this;
    return copy;
}

template<TopAbs_ShapeEnum shapeTag>
const typename McCAD::detail::TopClass<shapeTag>::type&
McCAD::Iterator<shapeTag>::operator*(){
    return (*detail::TopClass<shapeTag>::castFunction)(explorer->Current());
}
