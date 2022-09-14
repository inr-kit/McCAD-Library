// C++
#include <cmath>
#include <array>
#include <optional>
// McCAD
#include "surfaceComparator.hpp"
#include "planeComparator.hpp"
#include "cylinderComparator.hpp"
#include "coneComparator.hpp"
// OCCT
#include <TopLoc_Location.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>

McCAD::Tools::SurfaceComparator::SurfaceComparator(){}

McCAD::Tools::SurfaceComparator::SurfaceComparator(const double& precision,
                                                   const double& angularTolerance,
                                                   const double& distanceTolerance)
    : precision{precision}, angularTolerance{angularTolerance},
      distanceTolerance{distanceTolerance}{
}

McCAD::Tools::SurfaceComparator::~SurfaceComparator(){}

/** ********************************************************************
* @brief    An operator that compares two OCCT faces.
* @param    firstFace is a OCCT face.
* @param    secondFace is a OCCT face.
* @return   true if the two faces are equal.
* @date     31/12/2020
* @modified 23/08/2022
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Tools::SurfaceComparator::operator()(const TopoDS_Face& firstFace,
                                            const TopoDS_Face& secondFace) const{
    // Compare orientations
    if(firstFace.Orientation() != secondFace.Orientation()) return false;

    TopLoc_Location firstLocation, secondLocation;
    GeomAdaptor_Surface firstAdaptor{BRep_Tool::Surface(firstFace, firstLocation)};
    GeomAdaptor_Surface secondAdaptor{BRep_Tool::Surface(secondFace, secondLocation)};
    // Compare surfaces types.
    if(firstAdaptor.GetType() != secondAdaptor.GetType()) return false;
    // Compare locations
    if(firstLocation != secondLocation) return false;
    // Compare surfaces parameters
    if (firstAdaptor.GetType() == GeomAbs_Plane){
        auto condition = PlaneComparator{precision, angularTolerance,
                distanceTolerance}(firstAdaptor, secondAdaptor);
        if (condition) return condition.value();
        else return false;
    } else if (firstAdaptor.GetType() == GeomAbs_Cylinder){
        auto condition = CylinderComparator{precision, angularTolerance,
            distanceTolerance}(firstAdaptor, secondAdaptor);
        if (condition) return condition.value();
        else return false;
    } else if (firstAdaptor.GetType() == GeomAbs_Torus)
        return false; //torusComparator{}(firstAdaptor, secondAdaptor);
    else if (firstAdaptor.GetType() == GeomAbs_Cone) {
        auto condition = ConeComparator{ precision, angularTolerance,
            distanceTolerance }(firstAdaptor, secondAdaptor);
        if (condition) return condition.value();
        else return false;
    } else return false;
}
