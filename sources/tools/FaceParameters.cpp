#include "FaceParameters.hpp"

// OCC
#include <CSLib.hxx>

gp_Dir
McCAD::Tools::normalOnFace(
        const TopoDS_Face& face,
        const gp_Pnt& point){

    BRepAdaptor_Surface surfaceAdaptor{face, Standard_True};
    auto f_ptr = detail::getSurfFPtr(surfaceAdaptor.GetType());

    if(!f_ptr)
        return gp_Dir{1.0, 0.0, 0.0};

    auto direction
            = normalOnFace(
                calcDerivative(
                    surfaceAdaptor,
                    (*f_ptr)(surfaceAdaptor, point)
                    )
                );

    if (face.Orientation() == TopAbs_FORWARD)
        direction.Reverse();

    return direction;
}

gp_Dir
McCAD::Tools::normalOnFace(
        const DerivativeUV& derivative){
    gp_Dir normal;
    CSLib_DerivativeStatus dummyStatus;
    CSLib::Normal(derivative.u, derivative.v,
                  Precision::Confusion(),
                  dummyStatus,
                  normal);
    return normal;
}

McCAD::Tools::DerivativeUV
McCAD::Tools::calcDerivative(
        const BRepAdaptor_Surface& surface,
        const PositionUV& uv){
    gp_Pnt dummyPoint;
    DerivativeUV deriv;
    gp_Vec D1U, D1V;
    surface.D1(uv.u, uv.v,
               dummyPoint,
               deriv.u, deriv.v);
    return deriv;
}

McCAD::Tools::detail::SurfaceFunctionPointer
McCAD::Tools::detail::getSurfFPtr(
        GeomAbs_SurfaceType surfaceType){
    switch(surfaceType){
    case GeomAbs_Plane:
        return &toParameters<GeomAbs_Plane>;
    case GeomAbs_Cylinder:
        return &toParameters<GeomAbs_Cylinder>;
    case GeomAbs_Cone:
        return &toParameters<GeomAbs_Cone>;
    case GeomAbs_Sphere:
        return &toParameters<GeomAbs_Sphere>;
    case GeomAbs_Torus:
        return &toParameters<GeomAbs_Sphere>;
    default:
        return nullptr;
    }
}
