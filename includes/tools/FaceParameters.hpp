#ifndef FACEPARAMETERS_HPP
#define FACEPARAMETERS_HPP

#include "SurfaceUtilities.hpp"

// OCC
#include <gp_Dir.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <ElSLib.hxx>

namespace McCAD::Tools{

    struct PositionUV{
        Standard_Real u;
        Standard_Real v;
    };

    struct DerivativeUV{
        gp_Vec u;
        gp_Vec v;
    };

    gp_Dir normalOnFace(
            const TopoDS_Face& face,
            const gp_Pnt& point);

    gp_Dir normalOnFace(
            const DerivativeUV& derivative);

    DerivativeUV calcDerivative(
            const BRepAdaptor_Surface& surface,
            const PositionUV& uv);


    namespace detail{
        template<GeomAbs_SurfaceType surfaceType>
        PositionUV toParameters(const BRepAdaptor_Surface& surface,
                                 const gp_Pnt& point);

        using SurfaceFunctionPointer = PositionUV(*)(
            const BRepAdaptor_Surface&,
            const gp_Pnt&);

        SurfaceFunctionPointer getSurfFPtr(GeomAbs_SurfaceType surfaceType);
    }

}

template<GeomAbs_SurfaceType surfaceType>
McCAD::Tools::PositionUV
McCAD::Tools::detail::toParameters(
        const BRepAdaptor_Surface& surface,
        const gp_Pnt& point){
    PositionUV uv;
    ElSLib::Parameters(
                (surface.*gpSurfaceType<surfaceType>::conversionMemberFunction)(),
                point,
                uv.u, uv.v);
    return uv;
}

#endif
