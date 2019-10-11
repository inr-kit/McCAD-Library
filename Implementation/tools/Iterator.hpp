#ifndef ITERATOR_HPP
#define ITERATOR_HPP

// OCC
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

// C++
#include <memory>

namespace McCAD::detail{

    template<TopAbs_ShapeEnum shapeTag>
    struct TopClass;

    template<>
    struct TopClass<TopAbs_ShapeEnum::TopAbs_COMPOUND>{
        using type = TopoDS_Compound;
        using castFunctionType = const type&(*)(const TopoDS_Shape&);
        constexpr static const castFunctionType castFunction = &TopoDS::Compound;
    };

    template<>
    struct TopClass<TopAbs_ShapeEnum::TopAbs_COMPSOLID>{
        using type = TopoDS_CompSolid;
        using castFunctionType = const type&(*)(const TopoDS_Shape&);
        constexpr static const castFunctionType castFunction = &TopoDS::CompSolid;
    };

    template<>
    struct TopClass<TopAbs_ShapeEnum::TopAbs_SOLID>{
        using type = TopoDS_Solid;
        using castFunctionType = const type&(*)(const TopoDS_Shape&);
        constexpr static const castFunctionType castFunction = &TopoDS::Solid;
    };

    template<>
    struct TopClass<TopAbs_ShapeEnum::TopAbs_SHELL>{
        using type = TopoDS_Shell;
        using castFunctionType = const type&(*)(const TopoDS_Shape&);
        constexpr static const castFunctionType castFunction = &TopoDS::Shell;
    };

    template<>
    struct TopClass<TopAbs_ShapeEnum::TopAbs_FACE>{
        using type = TopoDS_Face;
        using castFunctionType = const type&(*)(const TopoDS_Shape&);
        constexpr static const castFunctionType castFunction = &TopoDS::Face;
    };

    template<>
    struct TopClass<TopAbs_ShapeEnum::TopAbs_WIRE>{
        using type = TopoDS_Wire;
        using castFunctionType = const type&(*)(const TopoDS_Shape&);
        constexpr static const castFunctionType castFunction = &TopoDS::Wire;
    };

    template<>
    struct TopClass<TopAbs_ShapeEnum::TopAbs_EDGE>{
        using type = TopoDS_Edge;
        using castFunctionType = const type&(*)(const TopoDS_Shape&);
        constexpr static const castFunctionType castFunction = &TopoDS::Edge;
    };

    template<>
    struct TopClass<TopAbs_ShapeEnum::TopAbs_VERTEX>{
        using type = TopoDS_Vertex;
        using castFunctionType = const type&(*)(const TopoDS_Shape&);
        constexpr static const castFunctionType castFunction = &TopoDS::Vertex;
    };

}

namespace McCAD{

    template<TopAbs_ShapeEnum shapeTag>
    class Iterator{

    public:
        using value_type = typename detail::TopClass<shapeTag>::type;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = std::size_t;
        using iterator_category = std::forward_iterator_tag;

        Iterator() = default;
        Iterator(const TopoDS_Shape& shape);

        bool operator==(const Iterator& that) const;
        bool operator!=(const Iterator& that) const;

        Iterator& operator++();
        Iterator operator++(int);

        const typename detail::TopClass<shapeTag>::type& operator*();

    private:
        std::shared_ptr<TopExp_Explorer> explorer;

    };

}

#include "Iterator.tpp"

#endif
