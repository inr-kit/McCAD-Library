// C++
#include <algorithm>
// McCAD
#include "solidsSorter.hpp"

/** ********************************************************************
* @brief   The function sorts a list of tuples using a given element index.
* @param   aList is a list of tuples to be sorted.
* @param   elementIndex is the index of element in a tuple to be used for sorting.
* @returns A sorted list of tuples.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
template<typename listType>
listType
McCAD::Conversion::SolidsSorter::sortByElement(listType& aList, const int& elementIndex){
    if      (elementIndex == 0) std::sort(aList.begin(), aList.end(), byLeft);
    else if (elementIndex == 2) std::sort(aList.begin(), aList.end(), byRight);
    else if (elementIndex == 3) std::sort(aList.begin(), aList.end(), byIntersection);
    return aList;
}

/** ********************************************************************
* @brief   The function sorts a list of tuples using a given element index.
* @param   aList is a list of tuples to be sorted.
* @param   elementIndex is the index of element in a tuple to be used for sorting.
* @returns A sorted list of tuples.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
template<typename listType>
listType
McCAD::Conversion::SolidsSorter::sortByElement2(listType& aList, const int& elementIndex){
    if      (elementIndex == 2) std::sort(aList.begin(), aList.end(), byIntersection2);
    else if (elementIndex == 3) std::sort(aList.begin(), aList.end(), bySplitting2);
    return aList;
}
