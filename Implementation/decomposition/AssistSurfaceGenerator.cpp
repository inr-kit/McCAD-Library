//C++
#include <math.h>
// McCAD
#include "AssistSurfaceGenerator.hpp"
#include "surfaceObjCerator.hpp"
#include "boundSurface_impl.hpp"
#include "ShapeView.hpp"
#include "senseEvaluateor.hpp"
//OCC
#include <AIS_AngleDimension.hxx>
#include <gp_Ax1.hxx>
#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>

void
McCAD::Decomposition::AssistSurfaceGenerator::operator()(
        Geometry::TORSolid& solidObj, Standard_Real angleTolerance){
    // Work on the case of partial torus; quarter for example.
    auto& planesList = solidObj.accessSImpl()->planesList;
    auto& toriList = solidObj.accessSImpl()->toriList;
    if (planesList.size() != 2) return;
    // Measure angle between extended surfaces
    AIS_AngleDimension angleDimension{planesList[0]->accessSImpl()->extendedFace,
                planesList[1]->accessSImpl()->extendedFace};
    auto radianAngle = angleDimension.GetValue();
    if (radianAngle <= angleTolerance) return;
    // Get axis of symmetry of torus and set rotation angle.
    gp_Ax1 axis = BRepAdaptor_Surface(toriList[0]->accessSImpl()->face).Torus().Axis();
    // Note: the method of determining the rotation sense has to be more robust.
    // could depend on axis direction, axises of surfaces.
    auto sense = Tools::SenseEvaluator{}(planesList[0]->accessSImpl()->face,
            BRepAdaptor_Surface(planesList[1]->accessSImpl()->face).Plane().Location());
    Standard_Real rotationSense = signbit(sense) ? -1.0 : +1.0;
    gp_Trsf rotation;
    rotation.SetRotation(axis, rotationSense*radianAngle/2.0);
    BRepBuilderAPI_Transform transform{rotation};
    transform.Perform(planesList[0]->accessSImpl()->face);
    auto assistShape = transform.ModifiedShape(planesList[0]->accessSImpl()->face);
    for(const auto& assistFace : detail::ShapeView<TopAbs_FACE>{assistShape}){
        auto assistSurfaceObj = SurfaceObjCreator{}(assistFace,
                                                    solidObj.accessSImpl()->boxDiagonalLength);
        solidObj.accessSImpl()->splitFacesList.push_back(std::move(assistSurfaceObj));
    }
    solidObj.accessSImpl()->splitSurface = Standard_True;
}
