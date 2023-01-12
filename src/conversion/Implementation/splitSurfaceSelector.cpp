// C++
#include <algorithm>
#include <string>
// McCAD
#include "splitSurfaceSelector.hpp"
#include "solidsSorter.hpp"

McCAD::Conversion::SplitSurfaceSelector::SplitSurfaceSelector(
        const int& maxSolidsPerVoidCell,
        const double& minVoidVolume) :
    maxSolidsPerVoidCell{maxSolidsPerVoidCell}, minVoidVolume{minVoidVolume} {}

McCAD::Conversion::SplitSurfaceSelector::~SplitSurfaceSelector(){}

/** ********************************************************************
* @brief   The function adds solids to the AABB of the void cell.
* @param   xMap, yMap, zMap: maps of solids.
* @param   voidCell is the void cell to be split.
* @returns A selected split surface.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
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
    // sort candidates by number of intersections.
    if(candidates.size() > 0){
        auto byIntersection = SolidsSorter{}.sortByElement2(candidates, 2);
        int returnIndex = 0;
        for(int i=1; i < byIntersection.size(); ++i){
            if(i == returnIndex) continue;
            if((std::get<2>(byIntersection[i]) == std::get<2>(byIntersection[returnIndex])) &&
                    (std::get<3>(byIntersection[i]) < std::get<3>(byIntersection[returnIndex])))
                returnIndex = i;
        }
        return byIntersection[returnIndex];
    }
    return std::nullopt;
}

/** ********************************************************************
* @brief   The function calculates the extent along the axis that corresponds to the minimum volume.
* @detal   Since the splitting will take place along one of the axes, the minimum void volume criteria
*          has to be translated to 1D. This is achieved by calculating the extent along each axis that 
*          corresponced to a minimum volume with the remaining axiex extents fixed.
* @param   voidCell is the void cell to be split.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::SplitSurfaceSelector::calcDimTolerances(
        const std::shared_ptr<VoidCell>& voidCell){
    double xExtent = voidCell->maxX - voidCell->minX;
    double yExtent = voidCell->maxY - voidCell->minY;
    double zExtent = voidCell->maxZ - voidCell->minZ;
    xTolerance = minVoidVolume / (yExtent * zExtent);
    yTolerance = minVoidVolume / (xExtent * zExtent);
    zTolerance = minVoidVolume / (xExtent * yExtent);
}

/** ********************************************************************
* @brief   The function selects candidate split surfaces.
* @param   aMap: a map of type McCAD::Conversion::SplitSurfaceSelector::dimMap.
* @param   aabbList is a tuple of the extent and center of the AABB of the void cell along a single axis.
* @param   dimTolerance is the 1D equivalent of the minimum void cell condition.
* @returns An optional selected split surface.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
std::optional<McCAD::Conversion::SplitSurfaceSelector::candidateTuple>
McCAD::Conversion::SplitSurfaceSelector::selectAxisSplitSurface(
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& aMap,
        const McCAD::Conversion::SplitSurfaceSelector::centerTuple& aabbList,
        const double& dimTolerance){
    candidateVec candidates;
    // Calculate the mean and standard deviation of the AABB centers.
    auto dist = calcCentersParameters(aMap);
    double minSplitDim;
    // TO set a criterial for split surface selection, a distinguish needs to be made between narrow
    // and wide distributions. This is achieved by judging whether the std of the AABB centers is less
    // than one third of the extent of the AABB of the void cell. This is an arbitrary condition!
    if(std::get<1>(dist) <= (std::get<2>(aabbList) - std::get<0>(aabbList))/3.0){
        // Narrow dist, sigma < extent of box / 3.
        double oneSigmaLeft = std::get<0>(dist) - std::get<1>(dist);
        minSplitDim = std::min(std::abs(oneSigmaLeft - std::get<0>(aabbList)),
                               std::abs(std::get<2>(aabbList) - oneSigmaLeft));
        if(oneSigmaLeft > std::get<0>(aabbList) && minSplitDim >= dimTolerance){
            // mu - sigma is inside aabb and within dimentional tolerance. Add to list.
            candidates.push_back(std::make_tuple(0, oneSigmaLeft, 0, 0, 0));
        }
        double oneSigmaRight = std::get<0>(dist) + std::get<1>(dist);
        minSplitDim = std::min(std::abs(oneSigmaRight - std::get<0>(aabbList)),
                               std::abs(std::get<2>(aabbList) - oneSigmaRight));
        if(oneSigmaRight < std::get<2>(aabbList) && minSplitDim >= dimTolerance){
            // mu + sigma is inside aabb and within dimentional tolerance. Add to list.
            candidates.push_back(std::make_tuple(0, oneSigmaRight, 0, 0, 0));
        }
    } else{
        // wide distribution.
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

/** ********************************************************************
* @brief   The function calculates the mean and standard deviation of the centers of solids' AABB.
* @param   aMap is a map of solids.
* @returns A tuple of the mean and standard deviation.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
std::tuple<double, double>
McCAD::Conversion::SplitSurfaceSelector::calcCentersParameters(
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& aMap){
    double sum{ 0.0 }, sum_squares{ 0 }, variance{ 0.0 }, stdDeviation;
    double numElements = aMap.size();
    for(const auto& element : aMap){
        sum += std::get<1>(element.second);
        sum_squares += std::pow(std::get<1>(element.second), 2);
    }
    double mean = sum / numElements;
    double mean_squares = sum_squares / numElements;
    variance = (numElements / (numElements - 1))*(mean_squares - std::pow(mean, 2));
    stdDeviation = std::sqrt(variance);
    return std::make_tuple(mean, stdDeviation);
}

/** ********************************************************************
* @brief   The function selects candidate split surfaces.
* @param   candidates: the candidate split surfaces.
* @param   aMap is a map of solids.
* @returns A selected split surface.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
McCAD::Conversion::SplitSurfaceSelector::candidateTuple
McCAD::Conversion::SplitSurfaceSelector::checkSplitSurfacePriority(
        McCAD::Conversion::SplitSurfaceSelector::candidateVec& candidates,
        const McCAD::Conversion::SplitSurfaceSelector::dimMap& aMap){
    for(auto& candidate : candidates){
        for(auto& element : aMap){
            if(std::get<0>(element.second) >= std::get<1>(candidate)){
                // min of the solid AABB is to the right of the splitting surface candidate.
                std::get<2>(candidate) += 1;
            } else if(std::get<2>(element.second) <= std::get<1>(candidate)){
                // max of the solid AABB is to the left of the splitting surface candidate.
                std::get<0>(candidate) += 1;
            } else {
                // solid intersects the spliting surface
                std::get<0>(candidate) += 1;
                std::get<2>(candidate) += 1;
                std::get<3>(candidate) += 1;
            }
        }
        // detect the maximum number of solids on either side of the splitting surface.
        double maxSolids = std::get<0>(candidate) >= std::get<2>(candidate) ?
                    std::get<0>(candidate) : std::get<2>(candidate);
        std::get<4>(candidate) = std::ceil(maxSolids / maxSolidsPerVoidCell);
    }
    // Order candidates per number of intersections.
    auto byIntersection = SolidsSorter{}.sortByElement(candidates, 3);
    // Check ordering by prospective subsequent splittings.
    int returnIndex = 0;
    if(byIntersection.size() > 1){
        for(int i=1; i < byIntersection.size(); ++i){
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
