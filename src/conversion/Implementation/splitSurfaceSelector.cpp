// C++
#include <algorithm>
#include <string>
// McCAD
#include "splitSurfaceSelector.hpp"
#include "solidsSorter.hpp"

McCAD::Conversion::SplitSurfaceSelector::SplitSurfaceSelector(
        const Standard_Integer& maxSolidsPerVoidCell,
        const Standard_Real& minVoidVolume) :
    maxSolidsPerVoidCell{maxSolidsPerVoidCell}, minVoidVolume{minVoidVolume} {}

McCAD::Conversion::SplitSurfaceSelector::~SplitSurfaceSelector(){}

std::optional<McCAD::Conversion::SplitSurfaceSelector::surfaceTuple>
McCAD::Conversion::SplitSurfaceSelector::process(
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& xMap,
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& yMap,
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& zMap,
        const std::shared_ptr<VoidCell>& voidCell){
    surfaceVec candidates;
    calcDimTolerances(voidCell);
    auto xSurface = selectAxisSplitSurface(xMap, voidCell->xAxis, xTolerance);
    if(xSurface) candidates.push_back(std::make_tuple("X", std::get<1>(*xSurface),
                                                      std::get<3>(*xSurface),
                                                      std::get<4>(*xSurface)));
    auto ySurface = selectAxisSplitSurface(yMap, voidCell->yAxis, yTolerance);
    if(ySurface) candidates.push_back(std::make_tuple("Y", std::get<1>(*ySurface),
                                                      std::get<3>(*ySurface),
                                                      std::get<4>(*ySurface)));
    auto zSurface = selectAxisSplitSurface(zMap, voidCell->zAxis, zTolerance);
    if(zSurface) candidates.push_back(std::make_tuple("Z", std::get<1>(*zSurface),
                                                      std::get<3>(*zSurface),
                                                      std::get<4>(*zSurface)));
    // sort candidates by number of intersections
    if(candidates.size() > 0){
        auto byIntersection = SolidsSorter{}.sortByElement2(candidates, 2);
        Standard_Integer returnIndex = 0;
        for(Standard_Integer i=1; i < byIntersection.size(); ++i){
            if(i == returnIndex) continue;
            if((std::get<2>(byIntersection[i]) == std::get<2>(byIntersection[returnIndex])) &&
                    (std::get<3>(byIntersection[i]) < std::get<3>(byIntersection[returnIndex])))
                returnIndex = i;
        }
        return byIntersection[returnIndex];
    }
    return std::nullopt;
}

void
McCAD::Conversion::SplitSurfaceSelector::calcDimTolerances(
        const std::shared_ptr<VoidCell>& voidCell){
    Standard_Real xExtent = voidCell->maxX - voidCell->minX;
    Standard_Real yExtent = voidCell->maxY - voidCell->minY;
    Standard_Real zExtent = voidCell->maxZ - voidCell->minZ;
    xTolerance = minVoidVolume / (yExtent * zExtent);
    yTolerance = minVoidVolume / (xExtent * zExtent);
    zTolerance = minVoidVolume / (xExtent * yExtent);
}

std::optional<McCAD::Conversion::SplitSurfaceSelector::candidateTuple>
McCAD::Conversion::SplitSurfaceSelector::selectAxisSplitSurface(
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& aMap,
        const McCAD::Conversion::SplitSurfaceSelector::centerTuple& aabbList,
        const Standard_Real& dimTolerance){
    candidateVec candidates;
    // Calculate the mean and standard deviation of the AABB centers.
    auto dist = calcCentersParameters(aMap);
    Standard_Real minSplitDim;
    if(std::get<1>(dist) <= (std::get<2>(aabbList) - std::get<0>(aabbList))/3.0){
        // Narrow dist, sigma < extent of box / 3.
        Standard_Real oneSigmeLeft = std::get<0>(dist) - std::get<1>(dist);
        minSplitDim = std::min(std::abs(oneSigmeLeft - std::get<0>(aabbList)),
                               std::abs(std::get<2>(aabbList) - oneSigmeLeft));
        if(oneSigmeLeft > std::get<0>(aabbList) && minSplitDim >= dimTolerance){
            // mu - sigma is inside aabb and within dimentional tolerance. Add to list.
            candidates.push_back(std::make_tuple(0, oneSigmeLeft, 0, 0, 0));
        }
        Standard_Real oneSigmeRight = std::get<0>(dist) + std::get<1>(dist);
        minSplitDim = std::min(std::abs(oneSigmeRight - std::get<0>(aabbList)),
                               std::abs(std::get<2>(aabbList) - oneSigmeRight));
        if(oneSigmeRight < std::get<2>(aabbList) && minSplitDim >= dimTolerance){
            // mu + sigma is inside aabb and within dimentional tolerance. Add to list.
            candidates.push_back(std::make_tuple(0, oneSigmeRight, 0, 0, 0));
        }
    } else{
        // wide distribution and mu within dimentional tolerance. Add to list.
        minSplitDim = std::min(std::abs(std::get<0>(dist) - std::get<0>(aabbList)),
                               std::abs(std::get<2>(aabbList) - std::get<0>(dist)));
        if(minSplitDim >= dimTolerance){
        candidates.push_back(std::make_tuple(0, std::get<0>(dist), 0, 0, 0));
        }
    }
    // Add middle of box as a candidate.
    minSplitDim = std::abs(std::get<1>(aabbList) - std::get<0>(aabbList));
    if(minSplitDim >= dimTolerance){
        candidates.push_back(std::make_tuple(0, std::get<1>(aabbList), 0, 0, 0));
    }
    if (candidates.size() > 0) return checkSplitSurfacePriority(candidates, aMap);
    return std::nullopt;
}

std::tuple<Standard_Real, Standard_Real>
McCAD::Conversion::SplitSurfaceSelector::calcCentersParameters(
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& aMap){
    Standard_Real sum{0.0}, sum_squares{0}, variance{0.0}, stdDeviation;
    Standard_Real numElements = aMap.size();
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
        Standard_Real maxSolids = std::get<0>(candidate) >= std::get<2>(candidate) ?
                    std::get<0>(candidate) : std::get<2>(candidate);
        std::get<4>(candidate) = std::ceil(maxSolids / maxSolidsPerVoidCell);
        }
    // Order candidates per number of intersections.
    auto byIntersection = SolidsSorter{}.sortByElement(candidates, 3);
    // Check ordering by prospective subsequent splittings
    Standard_Integer returnIndex = 0;
    if(byIntersection.size() > 1){
        for(Standard_Integer i=1; i < byIntersection.size(); ++i){
            if(i == returnIndex) continue;
            // A lower number of intersections takes precedence to the number of
            // subsequent splittings!. In case of an equal number of intersections,
            // choose the candidate with the lower number of subsequent splittings.
            if((std::get<3>(byIntersection[i]) == std::get<3>(byIntersection[returnIndex])) &&
                (std::get<4>(byIntersection[i]) < std::get<4>(byIntersection[returnIndex])))
                returnIndex = i;
        }
    }
    return byIntersection[returnIndex];
}
