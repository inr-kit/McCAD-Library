// C++
#include <cmath>
#include <array>
#include <optional>
// McCAD
#include "surfaceComparator.hpp"
#include "planeComparator.hpp"
#include "cylinderComparator.hpp"
#include "coneComparator.hpp"
#include "torusComparator.hpp"
// OCC
#include <TopLoc_Location.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>

McCAD::Tools::SurfaceComparator::SurfaceComparator(){}

McCAD::Tools::SurfaceComparator::SurfaceComparator(const Standard_Real& precision,
                                                   const Standard_Real& angularTolerance,
                                                   const Standard_Real& distanceTolerance)
    : precision{precision}, angularTolerance{angularTolerance},
      distanceTolerance{distanceTolerance}{
}

McCAD::Tools::SurfaceComparator::~SurfaceComparator(){}

Standard_Boolean
McCAD::Tools::SurfaceComparator::operator()(const TopoDS_Face& firstFace,
                                            const TopoDS_Face& secondFace) const{
    // Compare orientations
    if(firstFace.Orientation() != secondFace.Orientation()) return Standard_False;

    TopLoc_Location firstLocation, secondLocation;
    GeomAdaptor_Surface firstAdaptor{BRep_Tool::Surface(firstFace, firstLocation)};
    GeomAdaptor_Surface secondAdaptor{BRep_Tool::Surface(secondFace, secondLocation)};
    // Compare surfaces types.
    if(firstAdaptor.GetType() != secondAdaptor.GetType()) return Standard_False;
    // Compare locations
    if(firstLocation != secondLocation) return Standard_False;
    // Compare surfaces parameters
    if (firstAdaptor.GetType() == GeomAbs_Plane){
        auto condition = PlaneComparator{precision, angularTolerance,
                distanceTolerance}(firstAdaptor, secondAdaptor);
        if (condition) return condition.value();
        else return Standard_False;
    } else if (firstAdaptor.GetType() == GeomAbs_Cylinder){
        auto condition = CylinderComparator{precision, angularTolerance,
            distanceTolerance}(firstAdaptor, secondAdaptor);
        if (condition) return condition.value();
        else return Standard_False;
    } else if (firstAdaptor.GetType() == GeomAbs_Torus) {
        auto condition = TorusComparator{ precision, angularTolerance,
            distanceTolerance }(firstAdaptor, secondAdaptor);
        if (condition) return condition.value();
        else return Standard_False;
    } else if (firstAdaptor.GetType() == GeomAbs_Cone) {
        auto condition = ConeComparator{ precision, angularTolerance,
            distanceTolerance }(firstAdaptor, secondAdaptor);
        if (condition) return condition.value();
        else return Standard_False;
    }
    else return Standard_False;
}
