// McCAD
#include "faceParameters.hpp"
// OCC
#include <BRep_Tool.hxx>
#include <Standard.hxx>
#include <gp_Ax1.hxx>
#include <CSLib.hxx>
#include <ElSLib.hxx>

McCAD::Tools::FaceParameters::FaceParameters(){
}

McCAD::Tools::FaceParameters::FaceParameters(const Standard_Real& precision) :
    precision{precision}{}

McCAD::Tools::FaceParameters::~FaceParameters(){}

std::optional<gp_Dir>
McCAD::Tools::FaceParameters::normalOnFace(const TopoDS_Face& face,
                                           const gp_Pnt& point){
    BRepAdaptor_Surface surfaceAdaptor{face, Standard_True};
    auto uv = getSurfPositionUV(surfaceAdaptor, point);
    if(!uv) return std::nullopt;
    gp_Dir direction = normalOnFace(calcDerivative(surfaceAdaptor, *uv));
    // Reverse direction of normal to make it pointing always to material solid
    if (face.Orientation() == TopAbs_FORWARD) direction.Reverse();
    return direction;
}

std::optional<McCAD::Tools::FaceParameters::PositionUV>
McCAD::Tools::FaceParameters::getSurfPositionUV(const BRepAdaptor_Surface& surfaceAdaptor,
                                                const gp_Pnt& point){
    PositionUV uv;
    switch(surfaceAdaptor.GetType()){
    case GeomAbs_Plane:
        ElSLib::Parameters(
                    (surfaceAdaptor.*gpSurfaceType<GeomAbs_Plane>::conversionMemberFunction)(),
                    point, uv.u, uv.v);
        return uv;
    case GeomAbs_Cylinder:
        ElSLib::Parameters(
                    (surfaceAdaptor.*gpSurfaceType<GeomAbs_Cylinder>::conversionMemberFunction)(),
                    point, uv.u, uv.v);
        return uv;
    case GeomAbs_Torus:
        ElSLib::Parameters(
                    (surfaceAdaptor.*gpSurfaceType<GeomAbs_Torus>::conversionMemberFunction)(),
                    point, uv.u, uv.v);
        return uv;
    default:;
    }
    return std::nullopt;
}

gp_Dir
McCAD::Tools::FaceParameters::normalOnFace(const DerivativeUV& derivative){
    gp_Dir normal;
    CSLib_DerivativeStatus dummyStatus;
    CSLib::Normal(derivative.u, derivative.v, Precision::Confusion(), dummyStatus,
                  normal);
    return normal;
}

McCAD::Tools::FaceParameters::DerivativeUV
McCAD::Tools::FaceParameters::calcDerivative(const BRepAdaptor_Surface& surface,
                                             const PositionUV& uv){
    gp_Pnt dummyPoint;
    DerivativeUV deriv;
    surface.D1(uv.u, uv.v, dummyPoint, deriv.u, deriv.v);
    return deriv;
}

McCAD::Tools::FaceParameters::surfPrmts
McCAD::Tools::FaceParameters::genPlSurfParmts(const TopoDS_Face& face){
    // std::vector<gp_Pln, gp_Pnt, gp_Dir, parameters>
    gp_Ax1 planeNormal;
    TopLoc_Location location;
    std::array<Standard_Real, 4> planeParameters;
    GeomAdaptor_Surface surface{BRep_Tool::Surface(face, location)};
    gp_Pln plane = surface.Plane();
    if (face.Orientation() == TopAbs_REVERSED){
        planeNormal = plane.Axis();
        planeNormal.Reverse();
        plane.SetAxis(planeNormal);
    }
    plane.Coefficients(planeParameters[0], planeParameters[1], planeParameters[2],
                       planeParameters[3]);
    for(auto& parameter : planeParameters){
        if(std::abs(parameter) < precision) parameter = 0.0;
    }
    plane = gp_Pln(planeParameters[0], planeParameters[1], planeParameters[2],
                   planeParameters[3]);
    planeNormal = plane.Axis();
    surfPrmts generatedParmts;
    generatedParmts = std::make_tuple(plane, plane.Location(), planeNormal.Direction(), planeParameters);
    return generatedParmts;
}

void
McCAD::Tools::FaceParameters::genCylSurfParmts(const TopoDS_Face& face){
}

void
McCAD::Tools::FaceParameters::genTorSurfParmts(const TopoDS_Face& face){
}
