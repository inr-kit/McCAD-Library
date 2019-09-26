#ifndef SHAPEVIEW_HPP
#define SHAPEVIEW_HPP

#include "Iterator.hpp"

namespace McCAD{

    template<TopAbs_ShapeEnum shapeTag>
    class ShapeView{

    public:
        ShapeView(const TopoDS_Shape& shape);

        Iterator<shapeTag> begin() const;
        Iterator<shapeTag> end() const;
        std::size_t size() const;

    private:
        const TopoDS_Shape& shape;

    };
}

#include "ShapeView.tpp"

#endif
