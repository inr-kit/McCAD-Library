// C++
#include <algorithm>
// McCAD
#include "solidsSorter.hpp"

McCAD::Conversion::SolidsSorter::SolidsSorter(){}

McCAD::Conversion::SolidsSorter::~SolidsSorter(){}

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

Standard_Boolean
McCAD::Conversion::SolidsSorter::byIntersection2(
        const McCAD::Conversion::SolidsSorter::elementTuple2& first,
        const McCAD::Conversion::SolidsSorter::elementTuple2& second){
    return (std::get<2>(first) < std::get<2>(second));
}

Standard_Boolean
McCAD::Conversion::SolidsSorter::bySplitting2(
        const McCAD::Conversion::SolidsSorter::elementTuple2& first,
        const McCAD::Conversion::SolidsSorter::elementTuple2& second){
    return (std::get<3>(first) < std::get<3>(second));
}

