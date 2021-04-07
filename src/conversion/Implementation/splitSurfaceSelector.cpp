// C++
#include <algorithm>
#include <string>
// McCAD
#include "splitSurfaceSelector.hpp"
#include "solidsSorter.hpp"

McCAD::Conversion::SplitSurfaceSelector::SplitSurfaceSelector(
        const Standard_Integer& maxSolidsPerVoidCell) :
    maxSolidsPerVoidCell{maxSolidsPerVoidCell} {}

McCAD::Conversion::SplitSurfaceSelector::~SplitSurfaceSelector(){}

McCAD::Conversion::SplitSurfaceSelector::surfaceTuple
McCAD::Conversion::SplitSurfaceSelector::process(
        const McCAD::Conversion::SplitSurfaceSelector::dimList& xList,
        const McCAD::Conversion::SplitSurfaceSelector::dimList& yList,
        const McCAD::Conversion::SplitSurfaceSelector::dimList& zList,
        const std::shared_ptr<VoidCell>& voidCell){
    // elements in candidates tuple <type of surface, candidate, intersections,
    //                               maxProspectiveSplittings>
    McCAD::Conversion::SplitSurfaceSelector::surfaceVec candidates;
    auto xSurface = selectAxisSplitSurface(xList, voidCell->xAxis);
    candidates.push_back(std::make_tuple("X", std::get<1>(xSurface), std::get<3>(xSurface),
                                         std::get<4>(xSurface)));
    auto ySurface = selectAxisSplitSurface(yList, voidCell->yAxis);
    candidates.push_back(std::make_tuple("Y", std::get<1>(ySurface), std::get<3>(ySurface),
                                         std::get<4>(ySurface)));
    auto zSurface = selectAxisSplitSurface(zList, voidCell->zAxis);
    candidates.push_back(std::make_tuple("Z", std::get<1>(zSurface), std::get<3>(zSurface),
                                         std::get<4>(zSurface)));
    // sort candidates by number of intersections
    auto byIntersection = SolidsSorter{}.sortByElement2(candidates, 2);
    auto bySplitting = SolidsSorter{}.sortByElement2(candidates, 3);
    if(std::get<0>(byIntersection[0]) != std::get<0>(bySplitting[0]) &&
            std::get<2>(byIntersection[0]) == std::get<2>(bySplitting[0])){
        if (std::get<3>(byIntersection[0]) < std::get<3>(bySplitting[0])) return byIntersection[0];
        else return bySplitting[0];
    }
    std::cout << "X: " << std::get<1>(xSurface) << ", Inter: " << std::get<3>(xSurface) << ", next: " << std::get<4>(xSurface) <<
                 "\nY: " << std::get<1>(ySurface) << ", Inter: " << std::get<3>(ySurface) << ", next: " << std::get<4>(ySurface) <<
                 "\nZ: " << std::get<1>(zSurface) << ", Inter: " << std::get<3>(zSurface) << ", next: " << std::get<4>(zSurface) <<
                 std::endl;
    return byIntersection[0];
}

std::tuple<Standard_Real, Standard_Real>
McCAD::Conversion::SplitSurfaceSelector::calcCentersParameters(
        const McCAD::Conversion::SplitSurfaceSelector::dimList& list){
    Standard_Real sum{0.0}, sum_squares{0}, variance{0.0}, stdDeviation;
    Standard_Integer numElements = list.size();
    for(const auto& element : list){
        sum += std::get<2>(element);
        sum_squares += std::pow(std::get<2>(element), 2);
    }
    Standard_Real mean = sum / numElements;
    Standard_Real mean_squares = sum_squares / numElements;
    variance = (numElements / (numElements - 1))*(mean_squares - std::pow(mean, 2));
    stdDeviation = std::sqrt(variance);
    return std::make_tuple(mean, stdDeviation);
}

McCAD::Conversion::SplitSurfaceSelector::candidateTuple
McCAD::Conversion::SplitSurfaceSelector::checkSplitSurfacePriority(
        McCAD::Conversion::SplitSurfaceSelector::candidateVec& candidates,
        const McCAD::Conversion::SplitSurfaceSelector::dimList& list){
    // elements in list: <solidID, min, center, max>
    // candidate in candidates: <solids on left, candidate, solids on right,
    //                           intersections, maxProspectiveSplittings>
    for(auto& candidate : candidates){
        for(auto& element : list){
            if(std::get<1>(element) >= std::get<1>(candidate)){
                // min of box on right of splitting candidate
                std::get<2>(candidate) += 1;
            } else if(std::get<3>(element) <= std::get<1>(candidate)){
                // max of box on left of splitting candidate
                std::get<0>(candidate) += 1;
            } else {
                // solid intersects the spliting surface
                std::get<0>(candidate) += 1;
                std::get<2>(candidate) += 1;
                std::get<3>(candidate) += 1;
            }
        }
        Standard_Integer maxSolids{0};
        if(std::get<0>(candidate) >= std::get<2>(candidate)) maxSolids = std::get<0>(candidate);
        else maxSolids = std::get<2>(candidate);
        std::get<4>(candidate) = maxSolids / maxSolidsPerVoidCell ;
        }
    // Choose the best candidate
    auto byIntersection = SolidsSorter{}.sortByElement(candidates, 3);
    // Check ordering by prospective subsequent splittings
    if(byIntersection.size() > 1 && std::get<3>(byIntersection[0]) == std::get<3>(byIntersection[1])){
        if(std::get<4>(byIntersection[1]) < std::get<4>(byIntersection[0])){
            return byIntersection[1];
        }
    }
    return byIntersection[0];
}

McCAD::Conversion::SplitSurfaceSelector::candidateTuple
McCAD::Conversion::SplitSurfaceSelector::selectAxisSplitSurface(
        const McCAD::Conversion::SplitSurfaceSelector::dimList& list,
        const McCAD::Conversion::SplitSurfaceSelector::centerTuple& aabbList){
    McCAD::Conversion::SplitSurfaceSelector::candidateVec candidates;
    // Calculate the mean and standard deviation of the AABB centers.
    auto dist = calcCentersParameters(list);
    if(std::get<1>(dist) <= (std::get<2>(aabbList) - std::get<0>(aabbList))/3.0){
        if((std::get<0>(dist) - std::get<1>(dist)) > std::get<0>(aabbList)){
            candidates.push_back(std::make_tuple(0, std::get<0>(dist) - std::get<1>(dist), 0, 0, 0));
        }
        if((std::get<0>(dist) + std::get<1>(dist)) < std::get<2>(aabbList)){
            candidates.push_back(std::make_tuple(0, std::get<0>(dist) + std::get<1>(dist), 0, 0, 0));
        }
    } else {
        candidates.push_back(std::make_tuple(0, std::get<0>(dist), 0, 0, 0));
    }
    candidates.push_back(std::make_tuple(0, std::get<1>(aabbList), 0, 0, 0));
    return checkSplitSurfacePriority(candidates, list);
}
