//McCAD
#include "CommonEdgeFinder.hpp"
#include "EdgesCombiner.hpp"

std::vector<std::shared_ptr<McCAD::Geometry::Edge>>
McCAD::Decomposition::CommonEdgeFinder::operator()(
        const std::shared_ptr<McCAD::Geometry::BoundSurface> firstFace,
        const std::shared_ptr<McCAD::Geometry::BoundSurface> secondFace){
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    auto& firstEdgesList = firstFace->accessBSImpl()->edgesList;
    auto& secondEdgesList = secondFace->accessBSImpl()->edgesList;
    for (Standard_Integer i = 0; i < firstEdgesList.size(); ++i){
        for (Standard_Integer j = 0; j < secondEdgesList.size(); ++j){
            if (*(firstEdgesList[i]) == *(secondEdgesList[j])){
                commonEdges.push_back(firstEdgesList[i]);
            }
        }
    }
    EdgesCombiner{}(commonEdges);
    return commonEdges;
}
