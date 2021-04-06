// C++
#include <algorithm>
// McCAD
#include "splitSurfaceSelector.hpp"
#include "solidsSorter.hpp"

McCAD::Conversion::SplitSurfaceSelector::SplitSurfaceSelector(){}

McCAD::Conversion::SplitSurfaceSelector::~SplitSurfaceSelector(){}

void
McCAD::Conversion::SplitSurfaceSelector::process(
        const McCAD::Conversion::SplitSurfaceSelector::dimList& xList,
        const McCAD::Conversion::SplitSurfaceSelector::dimList& yList,
        const McCAD::Conversion::SplitSurfaceSelector::dimList& zList,
        const std::shared_ptr<VoidCell>& voidCell){
    auto xSurface = selectAxisSplitSurface(xList, voidCell->xAxis);
    auto ySurface = selectAxisSplitSurface(yList, voidCell->yAxis);
    auto zSurface = selectAxisSplitSurface(zList, voidCell->zAxis);
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

std::tuple<Standard_Real, Standard_Integer>
McCAD::Conversion::SplitSurfaceSelector::selectAxisSplitSurface(
        const McCAD::Conversion::SplitSurfaceSelector::dimList& list,
        const McCAD::Conversion::SplitSurfaceSelector::centerTuple& aabbList){
    Standard_Real leftMu, rightMu, middle;
    auto dist = calcCentersParameters(list);
    auto byMin{list}, byCent{list}, byMax{list};
    McCAD::Conversion::SolidsSorter{}.sortByElement(byMin, 1);
    McCAD::Conversion::SolidsSorter{}.sortByElement(byCent, 2);
    McCAD::Conversion::SolidsSorter{}.sortByElement(byMax, 3);
    if(std::get<1>(dist) <= std::get<1>(aabbList)/2.0){
        leftMu = std::get<0>(dist) - std::get<1>(dist);
        rightMu = std::get<0>(dist) + std::get<1>(dist);
    } else {
        leftMu = std::get<0>(dist);
        rightMu = std::get<0>(dist);
    }
    middle = std::get<1>(aabbList);
    // Get number of intersections
    std::cout << "Mu: " << std::get<0>(dist) << ", L: " << leftMu << ", M: " <<
                 middle << ", R: " << rightMu << std::endl;
    return std::make_tuple(0.0, 0);
}
