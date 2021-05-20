#ifndef SOLIDOBJCREATOR_HPP
#define SOLIDOBJCREATOR_HPP

//C++
#include <memory>
//OCC
#include <Standard.hxx>
#include <TopoDS_Shape.hxx>

namespace McCAD::Decomposition{
    class SolidObjCreator{
    public:
      template<typename solidObjType>
      std::shared_ptr<solidObjType> createObj(const TopoDS_Shape& shape,
                                              const Standard_Real& scalingFactor,
                                              const Standard_Real& angularTolerance,
                                              const Standard_Real& precision,
                                              const Standard_Real& edgeTolerance,
                                              const Standard_Real& faceTolerance);
    };
}

//McCAD
#include "solidObjCreator.tpp"

#endif //SOLIDOBJCREATOR_HPP
