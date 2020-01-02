// McCAD
#include "AssistSurfaceGenerator.hpp"
//OCC
#include <AIS_AngleDimension.hxx>

void
McCAD::Decomposition::AssistSurfaceGenerator::operator()(
        Geometry::TORSolid& solidObj){
    // Work on case of partial torus; quarter for ex.
    auto& planesList = solidObj.accessSImpl()->planesList;
    if (planesList.size() == 2){
        // measure angle between extended surfaces
        AIS_AngleDimension angleDimension{planesList[0]->accessSImpl()->extendedFace,
                    planesList[1]->accessSImpl()->extendedFace};
        auto radianAngle = angleDimension.GetValue();
        // create an extended plane middistance between the measured planes.

    }

}

