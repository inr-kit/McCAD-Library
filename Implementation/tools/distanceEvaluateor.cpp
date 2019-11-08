// McCAD
#include "distanceEvaluateor.hpp"
//OCC
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>

Standard_Real
McCAD::Tools::DistanceEvaluator::operator()(const TopoDS_Face& face,
                                            const gp_Pnt& point){
    GeomAdaptor_Surface surfaceAdaptor{BRep_Tool::Surface(face)};
    if (surfaceAdaptor.GetType() == GeomAbs_Plane){
        gp_Pln plane = surfaceAdaptor.Plane();
        return distanceToPlane(plane, point);
    } else if (surfaceAdaptor.GetType() == GeomAbs_Cylinder){
        gp_Cylinder cylinder = surfaceAdaptor.Cylinder();
        return distanceToCyl(cylinder, point);
    }
    else return 0;
}

Standard_Real
McCAD::Tools::DistanceEvaluator::distanceToPlane(const gp_Pln& plane,
                                                 const gp_Pnt& point){
    std::array<Standard_Real, 4> parameters;
    //A * X + B * Y + C * Z + D = 0
    plane.Coefficients(parameters[0], parameters[1], parameters[2], parameters[3]);
    return parameters[0]*point.X() + parameters[1]*point.Y() +
           parameters[2]*point.Z() + parameters[3];
}

Standard_Real
McCAD::Tools::DistanceEvaluator::distanceToCyl(const gp_Cylinder& cylinder,
                                               const gp_Pnt& point){
    std::array<Standard_Real, 10> parameters;
    // A1.X**2 + A2.Y**2 + A3.Z**2 + 2.(B1.X.Y + B2.X.Z + B3.Y.Z) + 2.(C1.X + C2.Y + C3.Z) + D = 0.0
    cylinder.Coefficients(parameters[0], parameters[1], parameters[2],
                          parameters[3], parameters[4], parameters[5],
                          parameters[6], parameters[7], parameters[8],
                          parameters[9]);
    return parameters[0]*std::pow(point.X(), 2) + parameters[1]*std::pow(point.Y(), 2) +
            parameters[2]*std::pow(point.Z(), 2) + 2*parameters[3]*point.X()*point.Y() +
            2*parameters[4]*point.X()*point.Z() + 2*parameters[5]*point.Y()*point.Z()
            + parameters[3];
}
