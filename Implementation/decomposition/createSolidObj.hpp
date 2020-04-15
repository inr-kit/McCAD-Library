#ifndef SOLIDOBJCREATOR_HPP
#define SOLIDOBJCREATOR_HPP

//C++
#include <memory>
//OCC
#include <TopoDS_Shape.hxx>

namespace McCAD::Decomposition{
    class SolidObjCreator{
    public:
      SolidObjCreator() = default;

      template<typename solidObjType>
      std::shared_ptr<solidObjType> createObj(const TopoDS_Shape& shape);
    };
}

//McCAD
#include "createSolidObj.tpp"

#endif //SOLIDOBJCREATOR_HPP
