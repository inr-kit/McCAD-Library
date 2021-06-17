//C++
#include <math.h>
#include <memory>
#include <vector>
// McCAD
#include "AssistCylCylSurfaceGenerator.hpp"
#include "CommonEdgeFinder.hpp"
#include "EdgesCombiner.hpp"
#include "CurveUtilities.hpp"
#include "splitSurfacesGenerator.hpp"
//OCC
#include <Standard.hxx>
#include <GeomAbs_CurveType.hxx>

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
            commonEdges = CommonEdgeFinder{}(cylindersList[i], cylindersList[j]);
            if(commonEdges.size() == 1){
                if(commonEdges[0]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
                    std::cout << "single linear common edge" << std::endl;
                    SplitSurfaceGenerator{}.generatePlaneOnEdge(
                                cylindersList[i]->accessSImpl()->face,
                                cylindersList[j]->accessSImpl()->face,
                                commonEdges[0]);
                } else{
                    std::cout << "single curved common edge" << std::endl;
                }
            }
            else{
                std::cout << "more common edge" << std::endl;
            }
        }
    }
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
