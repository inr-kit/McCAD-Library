// McCAD
#include "AssistSurfaceGenerator.hpp"
#include "surfaceObjCerator.hpp"
#include "boundSurface_impl.hpp"
//OCC
#include <AIS_AngleDimension.hxx>
#include "gp_Ax1.hxx"
#include "GeomAdaptor_Surface.hxx"
#include "BRep_Tool.hxx"
#include "gp_Pln.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "STEPControl_Writer.hxx"

void
McCAD::Decomposition::AssistSurfaceGenerator::operator()(
        Geometry::TORSolid& solidObj){
    // Work on case of partial torus; quarter for ex.
    auto& planesList = solidObj.accessSImpl()->planesList;
    auto& toriList = solidObj.accessSImpl()->toriList;
    if (planesList.size() == 2 && toriList.size() > 0){
        // measure angle between extended surfaces
        AIS_AngleDimension angleDimension{planesList[0]->accessSImpl()->extendedFace,
                    planesList[1]->accessSImpl()->extendedFace};
        auto radianAngle = angleDimension.GetValue();
        // create an extended plane mid-distance between the measured planes.
        gp_Ax1 axis = GeomAdaptor_Surface{BRep_Tool::Surface(
                                            toriList[0]->accessSImpl()->face)}.Torus().Axis();
        std::cout << axis.Direction().X() << ", " << axis.Direction().Y()
                  << ", " << axis.Direction().Z() << std::endl;
        auto assistPlane = GeomAdaptor_Surface{BRep_Tool::Surface(
                    planesList[0]->accessSImpl()->face)}.Plane();
        assistPlane.Rotate(axis, radianAngle);
        std::cout << "distance: " << assistPlane.Distance(GeomAdaptor_Surface{
                  BRep_Tool::Surface(planesList[0]->accessSImpl()->face)}.Plane())
                  << std::endl;
        auto assistantFace = BRepBuilderAPI_MakeFace(assistPlane).Face();
        auto assistantSurfaceObj = SurfaceObjCreator{}(
                    assistantFace, solidObj.accessSImpl()->boxDiagonalLength);
        solidObj.accessSImpl()->splitFacesList.push_back(
                std::move(assistantSurfaceObj));
        solidObj.accessSImpl()->splitSurface = Standard_True;
        //debug
        STEPControl_Writer writer6;
        writer6.Transfer(assistantFace, STEPControl_StepModelType::STEPControl_AsIs);
        Standard_Integer kk = 0;
        std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/bbox/assistSurface";
        std::string suffix = ".stp";
        while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
            ++kk;
        }
        filename += std::to_string(kk);
        filename += suffix;
        writer6.Write(filename.c_str());
        //debug
        }
}

