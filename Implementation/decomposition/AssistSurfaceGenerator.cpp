//C++
#include <math.h>
// McCAD
#include "AssistSurfaceGenerator.hpp"
#include "surfaceObjCerator.hpp"
#include "boundSurface_impl.hpp"
#include "ShapeView.hpp"
#include "commonedgefinder.hpp"
#include "edgescombiner.hpp"
//OCC
#include <AIS_AngleDimension.hxx>
#include <gp_Ax1.hxx>
#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <STEPControl_Writer.hxx>

void
McCAD::Decomposition::AssistSurfaceGenerator::operator()(Geometry::CYLSolid& solidObj){
    auto& planesList = solidObj.accessSImpl()->planesList;
    auto& cylindersList = solidObj.accessSImpl()->cylindersList;
    if (cylindersList.size() >= 2){
        std::vector<std::shared_ptr<Geometry::Edge>> commonCurvedEdges;
        for(Standard_Integer i = 0; i < cylindersList.size(); ++i){
            for(Standard_Integer j = i+1; j < cylindersList.size(); ++j){
                std::cout << "i: " << i << " j, " << j << std::endl;
                if (*cylindersList[i] == *cylindersList[j]) continue;
                auto temp = CommonEdgeFinder{}(cylindersList[i], cylindersList[j]);
                commonCurvedEdges.insert(commonCurvedEdges.end(), temp.begin(), temp.end());
                /*//debug
                STEPControl_Writer writer10;
                writer10.Transfer(cylindersList[i]->accessSImpl()->face, STEPControl_StepModelType::STEPControl_AsIs);
                writer10.Transfer(cylindersList[j]->accessSImpl()->face, STEPControl_StepModelType::STEPControl_AsIs);
                Standard_Integer kk = 0;
                std::string filename = "/home/mharb/opt//McCAD_refactor/examples/bbox/cylcyl";
                std::string suffix = ".stp";
                while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                    ++kk;
                }
                filename += std::to_string(kk);
                filename += suffix;
                writer10.Write(filename.c_str());
                *///debug
            }
        }
        EdgesCombiner{}(commonCurvedEdges);
        //debug
        STEPControl_Writer writer11;
        for (Standard_Integer i = 0; i < commonCurvedEdges.size(); ++i){
            writer11.Transfer(commonCurvedEdges[i]->accessEImpl()->edge,
                              STEPControl_StepModelType::STEPControl_AsIs);
        }
        Standard_Integer kk = 0;
        std::string filename = "/home/mharb/opt/McCAD_refactor/examples/bbox/commoncurvededges";
        std::string suffix = ".stp";
        while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
            ++kk;
        }
        filename += std::to_string(kk);
        filename += suffix;
        writer11.Write(filename.c_str());
        //debug
    }
}

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
    // Calculate the sense of the surfaces relative to the axis of rotation.
    gp_XYZ firstVec = gp_Vec(axis.Location(),
                             BRepAdaptor_Surface(
                                 planesList[0]->accessSImpl()->face).Plane().Location()).XYZ();
    gp_XYZ secondVec = gp_Vec(axis.Location(),
                             BRepAdaptor_Surface(
                                  planesList[1]->accessSImpl()->face).Plane().Location()).XYZ();
    gp_Dir crossProd = gp_Dir(firstVec).Crossed(gp_Dir(secondVec));
    auto sense = axis.Direction().Dot(crossProd);
    Standard_Real rotationSense = signbit(sense) ? -1.0 : 1.0;
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
    //for(const auto& edge : detail::ShapeView<TopAbs_EDGE>{assistShape}){
    //    std::cout << Tools::toTypeName(BRepAdaptor_Curve(edge).GetType()) << std::endl;
    //}
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
    Standard_Integer kk = 0;
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
