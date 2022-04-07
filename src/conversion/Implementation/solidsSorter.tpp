// C++
#include <algorithm>
// McCAD
#include "solidsSorter.hpp"

template<typename listType>
listType
McCAD::Conversion::SolidsSorter::sortByElement(listType& aList, const int& elementIndex){
    if      (elementIndex == 0) std::sort(aList.begin(), aList.end(), byLeft);
    else if (elementIndex == 2) std::sort(aList.begin(), aList.end(), byRight);
    else if (elementIndex == 3) std::sort(aList.begin(), aList.end(), byIntersection);
    return aList;
}

template<typename listType>
listType
McCAD::Conversion::SolidsSorter::sortByElement2(listType& aList, const int& elementIndex){
    if      (elementIndex == 2) std::sort(aList.begin(), aList.end(), byIntersection2);
    else if (elementIndex == 3) std::sort(aList.begin(), aList.end(), bySplitting2);
    return aList;
}
