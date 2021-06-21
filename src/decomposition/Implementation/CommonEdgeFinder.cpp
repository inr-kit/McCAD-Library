//McCAD
#include "CommonEdgeFinder.hpp"
#include "EdgesComparator.hpp"
#include "EdgesCombiner.hpp"

McCAD::Decomposition::CommonEdgeFinder::CommonEdgeFinder(
        const Standard_Real& angularTolerance, const Standard_Real& distanceTolerance)
    : angularTolerance{angularTolerance}, distanceTolerance{distanceTolerance}{
}

McCAD::Decomposition::CommonEdgeFinder::~CommonEdgeFinder(){
}

std::vector<std::shared_ptr<McCAD::Geometry::Edge>>
McCAD::Decomposition::CommonEdgeFinder::operator()(
        const std::shared_ptr<McCAD::Geometry::BoundSurface> firstFace,
        const std::shared_ptr<McCAD::Geometry::BoundSurface> secondFace){
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    auto& firstEdgesList = firstFace->accessBSImpl()->edgesList;
    auto& secondEdgesList = secondFace->accessBSImpl()->edgesList;
    for (Standard_Integer i = 0; i < firstEdgesList.size(); ++i){
        for (Standard_Integer j = 0; j < secondEdgesList.size(); ++j){
            if(Tools::EdgesComparator{angularTolerance, distanceTolerance}(
                        firstEdgesList[i]->accessEImpl()->edge,
                        secondEdgesList[j]->accessEImpl()->edge)){
            //if (*(firstEdgesList[i]) == *(secondEdgesList[j])){
                commonEdges.push_back(firstEdgesList[i]);
            }
        }
    }
    EdgesCombiner{}(commonEdges);
    return commonEdges;
}
