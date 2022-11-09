// C++
#include <math.h>
#include <memory>
#include <vector>
// McCAD
#include "AssistSurfaceGenerator.hpp"
#include "AssistCylCylSurfaceGenerator.hpp"
#include "AssistCylTorSurfaceGenerator.hpp"
#include "AssistPlnCylSurfaceGenerator.hpp"
#include "AssistPlnConeSurfaceGenerator.hpp"
#include "AssistCylConSurfaceGenerator.hpp"
#include "AssistConConSurfaceGenerator.hpp"
#include "edge_impl.hpp"
#include "CommonEdgeFinder.hpp"
#include "EdgesCombiner.hpp"
#include "surfaceObjCerator.hpp"
#include "ShapeView.hpp"
#include "surfacesMerger.hpp"
#include "torusConvertor.hpp"
#include "faceParameters.hpp"
// OCCT
#include <PrsDim_AngleDimension.hxx>
#include <gp_Ax1.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_XYZ.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <gp_Torus.hxx>
#include <BRepBuilderAPI_Transform.hxx>
//#include <STEPControl_Writer.hxx>

McCAD::Decomposition::AssistSurfaceGenerator::AssistSurfaceGenerator(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{
}

McCAD::Decomposition::AssistSurfaceGenerator::~AssistSurfaceGenerator(){
}

void
McCAD::Decomposition::AssistSurfaceGenerator::operator()(Geometry::CYLSolid& solidObj){
    // Generate assistant surface that splits cylinders first.
    if (solidObj.accessSImpl()->cylindersList.size() >= 2){
        AssistCylCylSurfaceGenerator{inputConfig}(solidObj);
    }
    if (solidObj.accessSImpl()->cylindersList.size() >= 1 &&
            solidObj.accessSImpl()->planesList.size() >= 1){
        AssistPlnCylSurfaceGenerator{inputConfig}(solidObj);
    }
    SurfacesMerger{}(solidObj.accessSImpl()->assistFacesList,
            solidObj.accessSImpl()->boxDiagonalLength, inputConfig.precision,
            inputConfig.edgeTolerance, inputConfig.angularTolerance,
            inputConfig.distanceTolerance);
}

/** ********************************************************************
* @brief    This function deligates the creation of split surfaces to specialized functions.
* @param    solidObj is a McCAD mixed solid object.
* @date     08/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Decomposition::AssistSurfaceGenerator::operator()(Geometry::MXDSolid& solidObj){
    // Generate assistant surface that splits cylinders and tori first.
    if (solidObj.accessSImpl()->cylindersList.size() >= 1 &&
        solidObj.accessSImpl()->toriList.size() >= 1){
        AssistCylTorSurfaceGenerator{inputConfig}(solidObj);
    }
    if (solidObj.accessSImpl()->cylindersList.size() >= 1 &&
        solidObj.accessSImpl()->conesList.size() >= 1) {
        AssistCylConSurfaceGenerator{ inputConfig }(solidObj);
    }
    if (solidObj.accessSImpl()->toriList.size() >= 1 &&
        solidObj.accessSImpl()->conesList.size() >= 1) {
        // Assist surface for splitting a cylinder and a cone.
    }
    SurfacesMerger{}(solidObj.accessSImpl()->assistFacesList,
        solidObj.accessSImpl()->boxDiagonalLength, inputConfig.precision,
        inputConfig.edgeTolerance, inputConfig.angularTolerance,
        inputConfig.distanceTolerance);
}

void
McCAD::Decomposition::AssistSurfaceGenerator::operator()(Geometry::TORSolid& solidObj){
    // Work on the case of partial torus; quarter for example.
    auto& planesList = solidObj.accessSImpl()->planesList;
    auto& toriList = solidObj.accessSImpl()->toriList;
    if (planesList.size() != 2) return;
    if(!TorusConvertor{inputConfig}.convertCondition(solidObj.accessSImpl()->solidShape)) return;
    // Measure angle between extended surfaces
    PrsDim_AngleDimension angleDimension{planesList[0]->accessSImpl()->extendedFace,
                planesList[1]->accessSImpl()->extendedFace};
    auto radianAngle = angleDimension.GetValue();
    if (radianAngle <= inputConfig.torusSplitAngle) return;
    // Get axis of symmetry of torus and set rotation angle.
    gp_Ax1 axis = BRepAdaptor_Surface(toriList[0]->accessSImpl()->face).Torus().Axis();
    // Calculate the sense of the surfaces relative to the axis of rotation.
    gp_XYZ firstVec = gp_Vec(axis.Location(),
                             BRepAdaptor_Surface(
                                 planesList[0]->accessSImpl()->face).Plane().Location()).XYZ();
    gp_XYZ secondVec = gp_Vec(axis.Location(),
                             BRepAdaptor_Surface(
                                  planesList[1]->accessSImpl()->face).Plane().Location()).XYZ();
    gp_Dir crossProd = gp_Dir(firstVec).Crossed(gp_Dir(secondVec));
    auto sense = axis.Direction().Dot(crossProd);
    double rotationSense = signbit(sense) ? -1.0 : 1.0;
    gp_Trsf rotation;
    rotation.SetRotation(axis, rotationSense*radianAngle/2.0);
    BRepBuilderAPI_Transform transform{rotation};
    transform.Perform(planesList[0]->accessSImpl()->face);
    auto assistShape = transform.ModifiedShape(planesList[0]->accessSImpl()->face);
    for(const auto& assistFace : detail::ShapeView<TopAbs_FACE>{assistShape}){
        auto assistSurfaceObj = SurfaceObjCreator{}(assistFace,
                                                    solidObj.accessSImpl()->boxDiagonalLength,
                                                    inputConfig.edgeTolerance);
        solidObj.accessSImpl()->assistFacesList.push_back(std::move(assistSurfaceObj));
    }
    solidObj.accessSImpl()->splitSurface = true;
    /*//debug
    std::cout << "angle: " << radianAngle << std::endl;
    auto plane0 = BRepAdaptor_Surface(planesList[0]->accessSImpl()->face).Plane().Location();
    std::cout << "plane 0: " << plane0.X() << ", " << plane0.Y() << ", " << plane0.Z()
              << std::endl;
    auto plane1 = BRepAdaptor_Surface(planesList[1]->accessSImpl()->face).Plane().Location();
    std::cout << "plane 1: " << plane1.X() << ", " << plane1.Y() << ", " << plane1.Z()
              << std::endl;
    STEPControl_Writer writer9;
    writer9.Transfer(planesList[0]->accessSImpl()->face, STEPControl_StepModelType::STEPControl_AsIs);
    writer9.Transfer(planesList[1]->accessSImpl()->face, STEPControl_StepModelType::STEPControl_AsIs);
    writer9.Transfer(assistShape, STEPControl_StepModelType::STEPControl_AsIs);
    int kk = 0;
    std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/bbox/cyltor";
    std::string suffix = ".stp";
    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
        ++kk;
    }
    filename += std::to_string(kk);
    filename += suffix;
    writer9.Write(filename.c_str());
    *///debug
}

bool
McCAD::Decomposition::AssistSurfaceGenerator::checkFillet(Geometry::CYLSolid& solidObj){
    // Mark solid as fillet if it consists of two cylinders with no common edges
    // and four planar surfaces.
    if(solidObj.accessSImpl()->cylindersList.size() == 2 &&
            solidObj.accessSImpl()->planesList.size() == 4 &&
            solidObj.accessSImpl()->assistFacesList.size() == 0){
        auto& cylindersList = solidObj.accessSImpl()->cylindersList;
        // If any cylinder is more than 90 degrees, not a fillet.
        for(int i = 0; i < cylindersList.size(); ++i){
            if(std::abs(Tools::FaceParameters{}.getRadian(cylindersList[i]->accessSImpl()->face)
                    - inputConfig.PI/2.0) > inputConfig.precision) goto goback;
        }
        // Sense of surfaces should not be the same.
        if(cylindersList[0]->accessSImpl()->face.Orientation() ==
                cylindersList[1]->accessSImpl()->face.Orientation()) goto goback;
        return true;
    }
    goback:;
    return false;
}

/** ********************************************************************
* @brief    This function deligates the creation of split surfaces to specialized functions.
* @param    solidObj is a McCAD conical solid object.
* @date     04/11/2022
* @modified 
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Decomposition::AssistSurfaceGenerator::operator()(Geometry::CONSolid & solidObj) {
    // Generate assistant surfaces that splits cones first.
    if (solidObj.accessSImpl()->conesList.size() >= 2) {
        AssistConConSurfaceGenerator{ inputConfig }(solidObj);
    }
    if (solidObj.accessSImpl()->conesList.size() >= 1 &&
        solidObj.accessSImpl()->planesList.size() >= 1) {
        AssistPlnConeSurfaceGenerator{inputConfig}(solidObj);
    }
    SurfacesMerger{}(solidObj.accessSImpl()->assistFacesList,
        solidObj.accessSImpl()->boxDiagonalLength, inputConfig.precision,
        inputConfig.edgeTolerance, inputConfig.angularTolerance,
        inputConfig.distanceTolerance);
}