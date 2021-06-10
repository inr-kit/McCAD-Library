#ifndef SHAPEVIEW_HPP
#define SHAPEVIEW_HPP

// McCAD
#include "Iterator.hpp"
// OCC
#include <TopoDS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>

namespace McCAD::detail{

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
