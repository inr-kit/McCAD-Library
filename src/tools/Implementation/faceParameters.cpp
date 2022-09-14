// McCAD
#include "faceParameters.hpp"
// OCCT
#include <BRep_Tool.hxx>
#include <gp_Ax1.hxx>
#include <CSLib.hxx>
#include <ElSLib.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <BRepTools.hxx>

McCAD::Tools::FaceParameters::FaceParameters(){
}

McCAD::Tools::FaceParameters::FaceParameters(const double& precision) :
    precision{precision}{}

McCAD::Tools::FaceParameters::FaceParameters(const double& precision,
                                             const double& scalingFactor) :
    precision{precision}, scalingFactor{scalingFactor}{}

McCAD::Tools::FaceParameters::~FaceParameters(){}

std::optional<gp_Dir>
McCAD::Tools::FaceParameters::normalOnFace(const TopoDS_Face& face,
                                           const gp_Pnt& point){
    BRepAdaptor_Surface surfaceAdaptor{face, true};
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

McCAD::Tools::FaceParameters::planePrmts
McCAD::Tools::FaceParameters::genPlSurfParmts(const TopoDS_Face& face){
    // This function is currently only used for conversion. It implements scaling
    // of the face per the user desired units as specified in the config file.
    // std::tuple<gp_Pln, gp_Pnt, gp_Dir, parameters>
    gp_Ax1 planeNormal;
    std::array<double, 4> planeParameters;
    BRepAdaptor_Surface surface{face, true};
    gp_Pln plane = surface.Plane();
    if (face.Orientation() == TopAbs_REVERSED){
        planeNormal = plane.Axis();
        planeNormal.Reverse();
        plane.SetAxis(planeNormal);
    }
    if(scalingFactor != 1.0){
        // Scale the plane before generating parameters.
        plane.Scale(gp_Pnt{0.0, 0.0, 0.0}, scalingFactor);
    }
    // Coefficients
    // A * X + B * Y + C * Z + D = 0.0
    plane.Coefficients(planeParameters[0], planeParameters[1], planeParameters[2],
                       planeParameters[3]);
    for(auto& parameter : planeParameters){
        if(std::abs(parameter) < precision) parameter = 0.0;
    }
    plane = gp_Pln(planeParameters[0], planeParameters[1], planeParameters[2],
                   planeParameters[3]);
    planeNormal = plane.Axis();
    planePrmts generatedParmts;
    generatedParmts = std::make_tuple(plane, plane.Location(), planeNormal.Direction(), planeParameters);
    return generatedParmts;
}

McCAD::Tools::FaceParameters::cylinderPrmts
McCAD::Tools::FaceParameters::genCylSurfParmts(const TopoDS_Face& face){
    // This function is currently only used for conversion. It implements scaling
    // of the face per the user desired units as specified in the config file.
    // std::tuple<gp_Cylinder, gp_Pnt, gp_Dir, parameters, radius, sense>
    std::array<double, 10> cylinderParameters;
    BRepAdaptor_Surface surface{face, true};
    gp_Cylinder cylinder = surface.Cylinder();
    if(scalingFactor != 1.0){
        // Scale the cylinder before generating parameters.
        cylinder.Scale(gp_Pnt{0.0, 0.0, 0.0}, scalingFactor);
    }
    gp_Ax1 symmetryAxis = cylinder.Axis();
    double radius = cylinder.Radius();
    int sense = face.Orientation() == TopAbs_FORWARD ? -1 : +1;
    // Coefficients
    // A1.X**2 + A2.Y**2 + A3.Z**2 + 2.(B1.X.Y + B2.X.Z + B3.Y.Z) + 2.(C1.X + C2.Y + C3.Z) + D = 0.0
    cylinder.Coefficients(cylinderParameters[0], cylinderParameters[1],
                          cylinderParameters[2], cylinderParameters[3],
                          cylinderParameters[4], cylinderParameters[5],
                          cylinderParameters[6], cylinderParameters[7],
                          cylinderParameters[8], cylinderParameters[9]);
    for(auto& parameter : cylinderParameters){
        if(std::abs(parameter) < precision) parameter = 0.0;
    }
    cylinderPrmts generatedParmts;
    generatedParmts = std::make_tuple(cylinder, cylinder.Location(),
                                      symmetryAxis.Direction(), cylinderParameters,
                                      radius, sense);
    return generatedParmts;
}

double
McCAD::Tools::FaceParameters::getRadian(const TopoDS_Face& cylinder){
    std::array<double, 4> uvParameters;
    // UV parameters in class grom_Cylindrical_Surface: U1 = 0 and U2 = 2*PI.
    BRepTools::UVBounds(cylinder, uvParameters[0], uvParameters[1], uvParameters[2],
            uvParameters[3]);
    return std::abs(uvParameters[1] - uvParameters[0]);
}

McCAD::Tools::FaceParameters::torusPrmts
McCAD::Tools::FaceParameters::genTorSurfParmts(const TopoDS_Face& face){
    // This function is currently only used for conversion. It implements scaling
    // of the face per the user desired units as specified in the config file.
    // std::tuple<gp_Torus, gp_Pnt, gp_Dir, parameters, minorRadius, majorRadius, sense>
    BRepAdaptor_Surface surface{face, true};
    gp_Torus torus = surface.Torus();
    if(scalingFactor != 1.0){
        // Scale the torus before generating parameters.
        torus.Scale(gp_Pnt{0.0, 0.0, 0.0}, scalingFactor);
    }
    gp_Ax1 symmetryAxis = torus.Axis();
    double minorRadius{torus.MinorRadius()},
                  majorRadius{torus.MajorRadius()};
    int sense = face.Orientation() == TopAbs_FORWARD ? -1 : +1;
    // Coefficients
    TColStd_Array1OfReal torusCoeffs{1, 35};
    torus.Coefficients(torusCoeffs);
    std::vector<double> torusParameters;
    for(const auto& coeff : torusCoeffs){
        torusParameters.push_back(coeff);
    }
    for(auto& parameter : torusParameters){
        if(std::abs(parameter) < precision) parameter = 0.0;
    }
    torusPrmts generatedParmts;
    generatedParmts = std::make_tuple(torus, torus.Location(), symmetryAxis.Direction(),
                                      torusParameters, minorRadius, majorRadius, sense);
    return generatedParmts;
}

/** ********************************************************************
* @brief    A function that generates a conical surface parameters.
* @param    face is a OCCT face object.
* @return   a tuple of face parameters.
* @date     14/09/2022
* @modified 
* @author   Moataz Harb
* **********************************************************************/
McCAD::Tools::FaceParameters::conePrmts
McCAD::Tools::FaceParameters::genConeSurfParmts(const TopoDS_Face& face) {
    // This function is currently only used for conversion. It implements scaling
    // of the face per the user desired units as specified in the config file.
    // std::tuple<gp_Cylinder, gp_Pnt, gp_Dir, parameters, radius, sense>
    std::array<double, 10> coneParameters;
    BRepAdaptor_Surface surface{ face, true };
    gp_Cone cone = surface.Cone();
    if (scalingFactor != 1.0) {
        // Scale the cone before generating parameters.
        cone.Scale(gp_Pnt{ 0.0, 0.0, 0.0 }, scalingFactor);
    }
    gp_Ax1 symmetryAxis = cone.Axis();
    double radius = cone.RefRadius();
    int sense = face.Orientation() == TopAbs_FORWARD ? -1 : +1;
    // Coefficients
    // A1.X**2 + A2.Y**2 + A3.Z**2 + 2.(B1.X.Y + B2.X.Z + B3.Y.Z) + 2.(C1.X + C2.Y + C3.Z) + D = 0.0
    cone.Coefficients(coneParameters[0], coneParameters[1],
                      coneParameters[2], coneParameters[3],
                      coneParameters[4], coneParameters[5],
                      coneParameters[6], coneParameters[7],
                      coneParameters[8], coneParameters[9]);
    for (auto& parameter : coneParameters) {
        if (std::abs(parameter) < precision) parameter = 0.0;
    }
    conePrmts generatedParmts;
    generatedParmts = std::make_tuple(cone, cone.Location(), symmetryAxis.Direction(), 
                                      coneParameters, radius, sense);
    return generatedParmts;
}
