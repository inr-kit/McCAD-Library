#ifndef CURVEUTILITIES_HPP
#define CURVEUTILITIES_HPP

// OCC
#include <GeomAbs_CurveType.hxx>
#include <gp.hxx>
#include <BRepAdaptor_Curve.hxx>

// C++
#include <string>

namespace McCAD::Tools{

    std::string toTypeName(
            GeomAbs_CurveType curveType);


    template<GeomAbs_CurveType curveType>
    struct gpCurveType;

    template<>
    struct gpCurveType<GeomAbs_CurveType::GeomAbs_Line>{
        using type = gp_Lin;
        using conversionType = type(BRepAdaptor_Curve::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Curve::Line;
    };

    template<>
    struct gpCurveType<GeomAbs_CurveType::GeomAbs_Circle>{
        using type = gp_Circ;
        using conversionType = type(BRepAdaptor_Curve::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Curve::Circle;
    };

    template<>
    struct gpCurveType<GeomAbs_CurveType::GeomAbs_Ellipse>{
        using type = gp_Elips;
        using conversionType = type(BRepAdaptor_Curve::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Curve::Ellipse;
    };

    template<>
    struct gpCurveType<GeomAbs_CurveType::GeomAbs_Hyperbola>{
        using type = gp_Hypr;
        using conversionType = type(BRepAdaptor_Curve::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Curve::Hyperbola;
    };

    template<>
    struct gpCurveType<GeomAbs_CurveType::GeomAbs_Parabola>{
        using type = gp_Parab;
        using conversionType = type(BRepAdaptor_Curve::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Curve::Parabola;
    };

}

#endif
