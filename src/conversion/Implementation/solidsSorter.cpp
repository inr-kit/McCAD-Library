// C++
#include <algorithm>
// McCAD
#include "solidsSorter.hpp"

McCAD::Conversion::SolidsSorter::SolidsSorter(){}

McCAD::Conversion::SolidsSorter::~SolidsSorter(){}

void
McCAD::Conversion::SolidsSorter::sortByElement(McCAD::Conversion::SolidsSorter::dimList& aList,
                                               const Standard_Integer& elementIndex){
    if (elementIndex == 1) std::sort(aList.begin(), aList.end(), byMin);
    else if (elementIndex == 2) std::sort(aList.begin(), aList.end(), byCenter);
    else if (elementIndex == 3) std::sort(aList.begin(), aList.end(), byMax);
    else std::sort(aList.begin(), aList.end());
}

Standard_Boolean
McCAD::Conversion::SolidsSorter::byMin(const McCAD::Conversion::SolidsSorter::dimTuple& first,
                                       const McCAD::Conversion::SolidsSorter::dimTuple& second){
    return (std::get<1>(first) < std::get<1>(second));
}

Standard_Boolean
McCAD::Conversion::SolidsSorter::byCenter(const McCAD::Conversion::SolidsSorter::dimTuple& first,
                                       const McCAD::Conversion::SolidsSorter::dimTuple& second){
    return (std::get<2>(first) < std::get<2>(second));
}

Standard_Boolean
McCAD::Conversion::SolidsSorter::byMax(const McCAD::Conversion::SolidsSorter::dimTuple& first,
                                       const McCAD::Conversion::SolidsSorter::dimTuple& second){
    return (std::get<3>(first) < std::get<3>(second));
}

