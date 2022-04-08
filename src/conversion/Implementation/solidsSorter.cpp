// C++
#include <algorithm>
// McCAD
#include "solidsSorter.hpp"

McCAD::Conversion::SolidsSorter::SolidsSorter(){}

McCAD::Conversion::SolidsSorter::~SolidsSorter(){}

/** ********************************************************************
* @details The function compares two candidate split surface tuples based 
*          on the number of solids on the left.
* @param   first is a candidate split surface.
* @param   second is a candidate split surface.
* @returns true or false.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
bool
McCAD::Conversion::SolidsSorter::byLeft(
        const McCAD::Conversion::SolidsSorter::elementTuple& first,
        const McCAD::Conversion::SolidsSorter::elementTuple& second){
    return (std::get<0>(first) < std::get<0>(second));
}

/** ********************************************************************
* @details The function compares two candidate split surface tuples based
*          on the number of solids on the right.
* @param   first is a candidate split surface.
* @param   second is a candidate split surface.
* @returns true or false.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
bool
McCAD::Conversion::SolidsSorter::byRight(
        const McCAD::Conversion::SolidsSorter::elementTuple& first,
        const McCAD::Conversion::SolidsSorter::elementTuple& second){
    return (std::get<2>(first) < std::get<2>(second));
}

/** ********************************************************************
* @details The function compares two candidate split surface tuples based
*          on the number of intersections with the solids AABBs.
* @param   first is a candidate split surface.
* @param   second is a candidate split surface.
* @returns true or false.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
bool
McCAD::Conversion::SolidsSorter::byIntersection(
        const McCAD::Conversion::SolidsSorter::elementTuple& first,
        const McCAD::Conversion::SolidsSorter::elementTuple& second){
    return (std::get<3>(first) < std::get<3>(second));
}

/** ********************************************************************
* @details The function compares two candidate split surface tuples based
*          on the number of intersections with the solids AABBs.
* @param   first is a candidate split surface tuple.
* @param   second is a candidate split surface tuple.
* @returns true or false.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
bool
McCAD::Conversion::SolidsSorter::byIntersection2(
        const McCAD::Conversion::SolidsSorter::elementTuple2& first,
        const McCAD::Conversion::SolidsSorter::elementTuple2& second){
    return (std::get<2>(first) < std::get<2>(second));
}

/** ********************************************************************
* @details The function compares two candidate split surface tuples based
*          on the number of intersections with the solids AABBs.
* @param   first is a candidate split surface tuple.
* @param   second is a candidate split surface tuple.
* @returns true or false.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
bool
McCAD::Conversion::SolidsSorter::bySplitting2(
        const McCAD::Conversion::SolidsSorter::elementTuple2& first,
        const McCAD::Conversion::SolidsSorter::elementTuple2& second){
    return (std::get<3>(first) < std::get<3>(second));
}

