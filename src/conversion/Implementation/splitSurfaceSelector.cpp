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
    auto xyzDist = calcCentersParameters();
    auto xbyMin = xList;
    McCAD::Conversion::SolidsSorter{}.sortByElement(xbyMin, 1);
    std::cout << "x: " << xList.size() << std::endl;
    for(const auto& i : xList){
        std::cout << std::get<0>(i) << ", " << std::get<1>(i) << ", " <<
        std::get<2>(i) << ", " << std::get<3>(i) << std::endl;
    }
}

McCAD::Conversion::SplitSurfaceSelector::centersDist
McCAD::Conversion::SplitSurfaceSelector::calcCentersParameters(){
    McCAD::Conversion::SplitSurfaceSelector::centersDist result;
    result.push_back(std::make_tuple(0.0, 0.0));
    result.push_back(std::make_tuple(0.0, 0.0));
    result.push_back(std::make_tuple(0.0, 0.0));
    return result;
}


