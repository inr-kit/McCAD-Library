//McCAD
#include "CommonEdgeFinder.hpp"
#include "EdgesComparator.hpp"
#include "EdgesCombiner.hpp"

McCAD::Decomposition::CommonEdgeFinder::CommonEdgeFinder(
        const double& angularTolerance, const double& distanceTolerance,
        const double& precision) : angularTolerance{angularTolerance},
    distanceTolerance{distanceTolerance}, precision{precision}{
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
    for (int i = 0; i < firstEdgesList.size(); ++i){
        for (int j = 0; j < secondEdgesList.size(); ++j){
            if(Tools::EdgesComparator{angularTolerance, distanceTolerance,
                    precision}(firstEdgesList[i]->accessEImpl()->edge,
                               secondEdgesList[j]->accessEImpl()->edge)){
                commonEdges.push_back(firstEdgesList[i]);
            }
        }
    }
    EdgesCombiner{}(commonEdges);
    return commonEdges;
}
