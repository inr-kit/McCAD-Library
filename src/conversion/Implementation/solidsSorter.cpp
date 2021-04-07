// C++
#include <algorithm>
// McCAD
#include "solidsSorter.hpp"

McCAD::Conversion::SolidsSorter::SolidsSorter(){}

McCAD::Conversion::SolidsSorter::~SolidsSorter(){}

template<typename listType>
listType
McCAD::Conversion::SolidsSorter::sortByElement(
        listType& aList,
        const Standard_Integer& elementIndex){
    if (elementIndex == 0) std::sort(aList.begin(), aList.end(), byLeft);
    else if (elementIndex == 2) std::sort(aList.begin(), aList.end(), byRight);
    else if (elementIndex == 3) std::sort(aList.begin(), aList.end(), byIntersection);
    return aList;
}

Standard_Boolean
McCAD::Conversion::SolidsSorter::byLeft(
        const McCAD::Conversion::SolidsSorter::elementTuple& first,
        const McCAD::Conversion::SolidsSorter::elementTuple& second){
    return (std::get<0>(first) < std::get<0>(second));
}

Standard_Boolean
McCAD::Conversion::SolidsSorter::byRight(
        const McCAD::Conversion::SolidsSorter::elementTuple& first,
        const McCAD::Conversion::SolidsSorter::elementTuple& second){
    return (std::get<2>(first) < std::get<2>(second));
}

Standard_Boolean
McCAD::Conversion::SolidsSorter::byIntersection(
        const McCAD::Conversion::SolidsSorter::elementTuple& first,
        const McCAD::Conversion::SolidsSorter::elementTuple& second){
    return (std::get<3>(first) < std::get<3>(second));
}

