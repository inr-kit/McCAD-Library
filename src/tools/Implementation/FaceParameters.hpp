#ifndef FACEPARAMETERS_HPP
#define FACEPARAMETERS_HPP

// C++
#include <array>
#include <tuple>
// McCAD
#include "SurfaceUtilities.hpp"
// OCC
#include <CSLib.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
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

    using surfPrmts = std::tuple<gp_Pln, gp_Pnt, gp_Dir, std::array<Standard_Real, 4>>;
    gp_Dir normalOnFace(const TopoDS_Face& face, const gp_Pnt& point);
    gp_Dir normalOnFace(const DerivativeUV& derivative);
    DerivativeUV calcDerivative(const BRepAdaptor_Surface& surface,
                                const PositionUV& position);
    surfPrmts genPlSurfParmts(const TopoDS_Face& face,
                              const Standard_Real& parameterTolerance = 1.0e-7);
    void genCylSurfParmts(const TopoDS_Face& face,
                         const Standard_Real& parameterTolerance = 1.0e-7);
    void genTorSurfParmts(const TopoDS_Face& face,
                         const Standard_Real& parameterTolerance = 1.0e-7);

    namespace detail{
        template<GeomAbs_SurfaceType surfaceType>
        PositionUV toPositionUV(const BRepAdaptor_Surface& surface,
                                const gp_Pnt& point);
        using SurfaceFunctionPointer = PositionUV(*)(const BRepAdaptor_Surface&,
        const gp_Pnt&);
        SurfaceFunctionPointer getSurfFPtr(GeomAbs_SurfaceType surfaceType);
    }
}

template<GeomAbs_SurfaceType surfaceType>
McCAD::Tools::PositionUV
McCAD::Tools::detail::toPositionUV(const BRepAdaptor_Surface& surface,
                                   const gp_Pnt& point){
    PositionUV uv;
    ElSLib::Parameters(
                (surface.*gpSurfaceType<surfaceType>::conversionMemberFunction)(),
                point, uv.u, uv.v);
    return uv;
}

#endif
