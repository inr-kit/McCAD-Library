#ifndef SURFACETYPENAME_HPP
#define SURFACETYPENAME_HPP

// OCC
#include <GeomAbs_SurfaceType.hxx>
#include <gp.hxx>
#include <BRepAdaptor_Surface.hxx>

// C++
#include <string>

namespace McCAD::Tools{

    std::string toTypeName(
            GeomAbs_SurfaceType surfaceType);


    template<GeomAbs_SurfaceType surfaceType>
    struct gpSurfaceType;

    template<>
    struct gpSurfaceType<GeomAbs_SurfaceType::GeomAbs_Plane>{
        using type = gp_Pln;
        using conversionType = type(BRepAdaptor_Surface::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Surface::Plane;
    };

    template<>
    struct gpSurfaceType<GeomAbs_SurfaceType::GeomAbs_Cylinder>{
        using type = gp_Cylinder;
        using conversionType = type(BRepAdaptor_Surface::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Surface::Cylinder;
    };

    template<>
    struct gpSurfaceType<GeomAbs_SurfaceType::GeomAbs_Cone>{
        using type = gp_Cone;
        using conversionType = type(BRepAdaptor_Surface::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Surface::Cone;
    };

    template<>
    struct gpSurfaceType<GeomAbs_SurfaceType::GeomAbs_Sphere>{
        using type = gp_Sphere;
        using conversionType = type(BRepAdaptor_Surface::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Surface::Sphere;
    };

    template<>
    struct gpSurfaceType<GeomAbs_SurfaceType::GeomAbs_Torus>{
        using type = gp_Torus;
        using conversionType = type(BRepAdaptor_Surface::*)() const;
        constexpr static const conversionType conversionMemberFunction
            = &BRepAdaptor_Surface::Torus;
    };

}

#endif
