//McCAD
#include "EdgesCombiner.hpp"

void
McCAD::Decomposition::EdgesCombiner::operator()(
        std::vector<std::shared_ptr<McCAD::Geometry::Edge>>& edgesList){
    //std::cout << "size before: " << edgesList.size() << std::endl;
    for (Standard_Integer i = 0; i < edgesList.size(); ++i){
        for (Standard_Integer j = i+1; j < edgesList.size(); ++j){
            if (*(edgesList[i]) == *(edgesList[j])){
                /* //debug
                STEPControl_Writer writer1;
                writer1.Transfer(edgesList[j]->accessEImpl()->edge,
                                 STEPControl_StepModelType::STEPControl_AsIs);
                writer1.Transfer(edgesList[i]->accessEImpl()->edge,
                                 STEPControl_StepModelType::STEPControl_AsIs);
                Standard_Integer kk = 0;
                std::string filename = "/home/mharb/opt/McCAD_refactor/examples/bbox/edges";
                std::string suffix = ".stp";
                while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                    ++kk;
                }
                filename += std::to_string(kk);
                filename += suffix;
                writer1.Write(filename.c_str());
                */ //debug
                //std::cout << "combineEdges, equal, erase" << std::endl;
                edgesList.erase(edgesList.begin() + j);
                --j;
            }
        }
    }
    //std::cout << "size after: " << edgesList.size() << std::endl;
}
