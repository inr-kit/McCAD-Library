//C++
#include <math.h>
#include <memory>
#include <vector>
// McCAD
#include "AssistCylCylSurfaceGenerator.hpp"
#include "edge_impl.hpp"
#include "CommonEdgeFinder.hpp"
#include "EdgesCombiner.hpp"
#include "surfaceObjCerator.hpp"
#include "ShapeView.hpp"
//OCC
#include <AIS_AngleDimension.hxx>
#include <gp_Ax1.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_XYZ.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <gp_Torus.hxx>
#include <BRepBuilderAPI_Transform.hxx>
//#include <STEPControl_Writer.hxx>

McCAD::Decomposition::AssistCylCylSurfaceGenerator::AssistCylCylSurfaceGenerator(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{
}

McCAD::Decomposition::AssistCylCylSurfaceGenerator::~AssistCylCylSurfaceGenerator(){
}

void
McCAD::Decomposition::AssistCylCylSurfaceGenerator::operator()(
        Geometry::CYLSolid& solidObj){
    auto& cylindersList = solidObj.accessSImpl()->cylindersList;
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    for(Standard_Integer i = 0; i < cylindersList.size(); ++i){
        for(Standard_Integer j = i+1; j < cylindersList.size(); ++j){
            if (*cylindersList[i] == *cylindersList[j]) continue;
            auto temp = CommonEdgeFinder{}(cylindersList[i], cylindersList[j]);
            if(temp.size() == 1){
                //planeOnEdgeGenerator{}(cylindersList[i]->accessSImpl()->face,
                //        cylindersList[j]->accessSImpl()->face, temp[0]);
            }
            else{}
            //commonEdges.insert(commonEdges.end(), temp.begin(), temp.end());
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
    EdgesCombiner{}(commonEdges);
    // sort edges into respective types lists
    /*//debug
    STEPControl_Writer writer11;
    for (Standard_Integer i = 0; i < commonCurvedEdges.size(); ++i){
        writer11.Transfer(commonCurvedEdges[i]->accessEImpl()->edge,
                          STEPControl_StepModelType::STEPControl_AsIs);
    }
    Standard_Integer kk = 0;
    std::string filename = "/home/mharb/opt/McCAD_refactor/examples/bbox/commonedges";
    std::string suffix = ".stp";
    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
        ++kk;
    }
    filename += std::to_string(kk);
    filename += suffix;
    writer11.Write(filename.c_str());
    *///debug
}
