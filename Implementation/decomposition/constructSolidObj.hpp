#ifndef CONSTRUCTSOLIDOBJ_HPP
#define CONSTRUCTSOLIDOBJ_HPP

//C++
#include <memory>
//McCAD
#include "solid_impl.hpp"
#include "generateFacesList.hpp"
//OCC
#include <TopoDS_Shape.hxx>

namespace McCAD::Decomposition{
    class SolidObjConstructor{
    public:
      SolidObjConstructor() = default;
      
      template<typename solidObjType>
      std::shared_ptr<solidObjType> constructObj(const TopoDS_Shape& shape);
    };
}

//McCAD
#include "constructSolidObj.tpp"

#endif //CONSTRUCTSOLIDOBJ_HPP
