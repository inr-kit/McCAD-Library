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
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& xMap,
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& yMap,
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& zMap,
        const std::shared_ptr<VoidCell>& voidCell){
    surfaceVec candidates;
    auto xSurface = selectAxisSplitSurface(xMap, voidCell->xAxis);
    candidates.push_back(std::make_tuple("X", std::get<1>(xSurface), std::get<3>(xSurface),
                                         std::get<4>(xSurface)));
    auto ySurface = selectAxisSplitSurface(yMap, voidCell->yAxis);
    candidates.push_back(std::make_tuple("Y", std::get<1>(ySurface), std::get<3>(ySurface),
                                         std::get<4>(ySurface)));
    auto zSurface = selectAxisSplitSurface(zMap, voidCell->zAxis);
    candidates.push_back(std::make_tuple("Z", std::get<1>(zSurface), std::get<3>(zSurface),
                                         std::get<4>(zSurface)));
    // sort candidates by number of intersections
    auto byIntersection = SolidsSorter{}.sortByElement2(candidates, 2);
    Standard_Integer returnIndex = 0;
    for(Standard_Integer i=1; i < byIntersection.size(); ++i){
        if(i == returnIndex) continue;
        if((std::get<2>(byIntersection[i]) == std::get<2>(byIntersection[returnIndex])) &&
            (std::get<3>(byIntersection[i]) < std::get<3>(byIntersection[returnIndex]))) returnIndex = i;
    }
    std::cout << "X: " << std::get<1>(xSurface) << ", Inter: " << std::get<3>(xSurface) << ", next: " << std::get<4>(xSurface) <<
                 "\nY: " << std::get<1>(ySurface) << ", Inter: " << std::get<3>(ySurface) << ", next: " << std::get<4>(ySurface) <<
                 "\nZ: " << std::get<1>(zSurface) << ", Inter: " << std::get<3>(zSurface) << ", next: " << std::get<4>(zSurface) <<
                 std::endl;
    return byIntersection[returnIndex];
}

McCAD::Conversion::SplitSurfaceSelector::candidateTuple
McCAD::Conversion::SplitSurfaceSelector::selectAxisSplitSurface(
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& aMap,
        const McCAD::Conversion::SplitSurfaceSelector::centerTuple& aabbList){
    candidateVec candidates;
    // Calculate the mean and standard deviation of the AABB centers.
    auto dist = calcCentersParameters(aMap);
    if(std::get<1>(dist) <= (std::get<2>(aabbList) - std::get<0>(aabbList))/3.0){
        // Narrow dist, sigma < extent of box / 3.
        if((std::get<0>(dist) - std::get<1>(dist)) > std::get<0>(aabbList)){
            // if mu - sigma is inside aabb, add it to list.
            candidates.push_back(std::make_tuple(0, std::get<0>(dist) - std::get<1>(dist),
                                                 0, 0, 0));
        }
        if((std::get<0>(dist) + std::get<1>(dist)) < std::get<2>(aabbList)){
            // if mu + sigma is inside aabb, add it to list.
            candidates.push_back(std::make_tuple(0, std::get<0>(dist) + std::get<1>(dist),
                                                 0, 0, 0));
        }
    } else {
        // wide distribution, add mu to list.
        candidates.push_back(std::make_tuple(0, std::get<0>(dist), 0, 0, 0));
    }
    // Add middle ob box as a candidate.
    candidates.push_back(std::make_tuple(0, std::get<1>(aabbList), 0, 0, 0));
    return checkSplitSurfacePriority(candidates, aMap);
}

std::tuple<Standard_Real, Standard_Real>
McCAD::Conversion::SplitSurfaceSelector::calcCentersParameters(
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& aMap){
    Standard_Real sum{0.0}, sum_squares{0}, variance{0.0}, stdDeviation;
    Standard_Integer numElements = aMap.size();
    for(const auto& element : aMap){
        sum += std::get<1>(element.second);
        sum_squares += std::pow(std::get<1>(element.second), 2);
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
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& aMap){
    for(auto& candidate : candidates){
        for(auto& element : aMap){
            if(std::get<0>(element.second) >= std::get<1>(candidate)){
                // min of box on right of splitting candidate
                std::get<2>(candidate) += 1;
            } else if(std::get<2>(element.second) <= std::get<1>(candidate)){
                // max of box on left of splitting candidate
                std::get<0>(candidate) += 1;
            } else {
                // solid intersects the spliting surface
                std::get<0>(candidate) += 1;
                std::get<2>(candidate) += 1;
                std::get<3>(candidate) += 1;
            }
        }
        Standard_Real maxSolids{0};
        if(std::get<0>(candidate) > std::get<2>(candidate)) maxSolids = std::get<0>(candidate);
        else maxSolids = std::get<2>(candidate);
        std::get<4>(candidate) = std::ceil(maxSolids / maxSolidsPerVoidCell);
        }
    // Choose the best candidate
    auto byIntersection = SolidsSorter{}.sortByElement(candidates, 3);
    // Check ordering by prospective subsequent splittings
    Standard_Integer returnIndex = 0;
    if(byIntersection.size() > 1){
        for(Standard_Integer i=1; i < byIntersection.size(); ++i){
            if(i == returnIndex) continue;
            if((std::get<3>(byIntersection[i]) == std::get<3>(byIntersection[returnIndex])) &&
                (std::get<4>(byIntersection[i]) < std::get<4>(byIntersection[returnIndex]))) returnIndex = i;
        }
    }
    return byIntersection[returnIndex];
}
