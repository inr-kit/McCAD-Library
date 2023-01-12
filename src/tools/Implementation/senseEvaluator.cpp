// C++
#include <array>
// McCAD
#include "senseEvaluator.hpp"
//OCCT
#include <BRepAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <TopAbs_Orientation.hxx>
#include <gp_Ax1.hxx>

/** ********************************************************************
* @brief    An operator that deligates sense calculation to specialized functions.
* @param    face is a OCCT face.
* @param    point is a OCCT point.
* @return   calculated sense: +1/-1.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<double>
McCAD::Tools::SenseEvaluator::operator()(const TopoDS_Face& face,
                                         const gp_Pnt& point){
    BRepAdaptor_Surface surfaceAdaptor(face, true);
    if (surfaceAdaptor.GetType() == GeomAbs_Plane){
        gp_Pln plane = surfaceAdaptor.Plane();
        if(face.Orientation() == TopAbs_REVERSED){
            gp_Ax1 planeNormal = plane.Axis();
            plane.SetAxis(planeNormal.Reversed());
        }
        return senseRelativeToPlane(plane, point);
    } else if (surfaceAdaptor.GetType() == GeomAbs_Cylinder){
        gp_Cylinder cylinder = surfaceAdaptor.Cylinder();
        return senseRelativeToCyl(cylinder, point);
    } else if (surfaceAdaptor.GetType() == GeomAbs_Cone) {
        gp_Cone cone = surfaceAdaptor.Cone();
        return senseRelativeToCone(cone, point);
    } else return std::nullopt;
}

/** ********************************************************************
* @brief    A function that calculates a point's sense relative to a plane.
* @param    plane is a OCCT plane.
* @param    point is a OCCT point.
* @return   evaluated plane equation at the point.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
double
McCAD::Tools::SenseEvaluator::senseRelativeToPlane(const gp_Pln& plane,
                                                   const gp_Pnt& point){
    std::array<double, 4> Coefficients;
    //A * X + B * Y + C * Z + D = 0.0
    plane.Coefficients(Coefficients[0], Coefficients[1], Coefficients[2], Coefficients[3]);
    return Coefficients[0]*point.X() + Coefficients[1]*point.Y() +
        Coefficients[2]*point.Z() + Coefficients[3];
}

/** ********************************************************************
* @brief    A function that calculates a point's sense relative to a cylinder.
* @param    cylinder is a OCCT cylinder.
* @param    point is a OCCT point.
* @return   evaluated cylinder equation at the point.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
double
McCAD::Tools::SenseEvaluator::senseRelativeToCyl(const gp_Cylinder& cylinder,
                                                 const gp_Pnt& point){
    std::array<double, 10> Coefficients;
    // A1.X**2 + A2.Y**2 + A3.Z**2 + 2.(B1.X.Y + B2.X.Z + B3.Y.Z) + 2.(C1.X + C2.Y + C3.Z) + D = 0.0
    cylinder.Coefficients(Coefficients[0], Coefficients[1], Coefficients[2],
                          Coefficients[3], Coefficients[4], Coefficients[5],
                          Coefficients[6], Coefficients[7], Coefficients[8],
                          Coefficients[9]);
    return Coefficients[0]*std::pow(point.X(), 2) + Coefficients[1]*std::pow(point.Y(), 2) +
            Coefficients[2]*std::pow(point.Z(), 2) + 2*Coefficients[3]*point.X()*point.Y() +
            2*Coefficients[4]*point.X()*point.Z() + 2*Coefficients[5]*point.Y()*point.Z()
            + Coefficients[3];
}

/** ********************************************************************
* @brief    A function that calculates a point's sense relative to a cone.
* @param    cone is a OCCT cone.
* @param    point is a OCCT point.
* @return   evaluated cone equation at the point.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
double
McCAD::Tools::SenseEvaluator::senseRelativeToCone(const gp_Cone& cone,
                                                  const gp_Pnt& point) {
    std::array<double, 10> Coefficients;
    // A1.X**2 + A2.Y**2 + A3.Z**2 + 2.(B1.X.Y + B2.X.Z + B3.Y.Z) + 2.(C1.X + C2.Y + C3.Z) + D = 0.0
    cone.Coefficients(Coefficients[0], Coefficients[1], Coefficients[2],
                      Coefficients[3], Coefficients[4], Coefficients[5],
                      Coefficients[6], Coefficients[7], Coefficients[8],
                      Coefficients[9]);
    return Coefficients[0] * std::pow(point.X(), 2) + Coefficients[1] * std::pow(point.Y(), 2) +
        Coefficients[2] * std::pow(point.Z(), 2) + 2 * Coefficients[3] * point.X() * point.Y() +
        2 * Coefficients[4] * point.X() * point.Z() + 2 * Coefficients[5] * point.Y() * point.Z()
        + Coefficients[3];
}
