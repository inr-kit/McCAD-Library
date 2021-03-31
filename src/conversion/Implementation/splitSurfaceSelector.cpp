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
        const McCAD::Conversion::SplitSurfaceSelector::dimList& zList){
    auto xSurface = selectAxisSplitSurface(xList);
    auto ySurface = selectAxisSplitSurface(yList);
    auto zSurface = selectAxisSplitSurface(zList);
}

std::tuple<Standard_Real, Standard_Real>
McCAD::Conversion::SplitSurfaceSelector::calcCentersParameters(
        const McCAD::Conversion::SplitSurfaceSelector::dimList& list){
    Standard_Real sum{0.0}, mean, variance{0.0}, stdDeviation;
    Standard_Integer numElements = list.size();
    for(const auto& element : list){
        sum += std::get<2>(element);
        variance += std::pow(std::get<2>(element), 2);
    }
    mean = sum / numElements;
    stdDeviation = std::sqrt(variance - std::pow(mean, 2));
    stdDeviation *= numElements / (numElements - 1);
    return std::make_tuple(mean, stdDeviation);
}

std::tuple<Standard_Real, Standard_Integer>
McCAD::Conversion::SplitSurfaceSelector::selectAxisSplitSurface(
        const McCAD::Conversion::SplitSurfaceSelector::dimList& list){
    auto dist = calcCentersParameters(list);
    auto xbyCent = list;
    McCAD::Conversion::SolidsSorter{}.sortByElement(xbyCent, 2);
    std::cout << "list: " << list.size() << std::endl;
    for(const auto& i : xbyCent){
        std::cout << std::get<0>(i) << ", " << std::get<1>(i) << ", " <<
        std::get<2>(i) << ", " << std::get<3>(i) << std::endl;
    }
    std::cout << "mean: " << std::get<0>(dist) << ", std: " << std::get<1>(dist) << std::endl;
    return std::make_tuple(0.0, 0);
}
