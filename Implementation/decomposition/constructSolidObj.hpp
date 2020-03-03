#ifndef CONSTRUCTSOLIDOBJ_HPP
#define CONSTRUCTSOLIDOBJ_HPP

//C++
#include <memory>
//OCC
#include <TopoDS_Shape.hxx>

namespace McCAD::Decomposition{
template<typename SolidObjType>
std::shared_ptr<SolidObjType> constructObj(const TopoDS_Shape& shape);
}

//McCAD
#include "constructSolidObj.tpp"

#endif //CONSTRUCTSOLIDOBJ_HPP
