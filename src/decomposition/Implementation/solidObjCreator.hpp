#ifndef SOLIDOBJCREATOR_HPP
#define SOLIDOBJCREATOR_HPP

//C++
#include <memory>
// McCAD
#include "inputconfig.hpp"
//OCC
#include <Standard.hxx>
#include <TopoDS_Shape.hxx>

namespace McCAD::Decomposition{
    class SolidObjCreator{
    public:
        SolidObjCreator(const IO::InputConfig& inputConfig);
        ~SolidObjCreator();
    private:
        Standard_Real scalingFactor{100.0},
                      angularTolerance{1.0e-4 * M_PI},
                      precision{1.0e-6},
                      edgeTolerance{1.0e-8},
                      faceTolerance{1.0e-8},
                      distanceTolerance{1.0e-6},
                      parameterTolerance{1.0e-7};
    public:
        template<typename solidObjType>
        std::shared_ptr<solidObjType> createObj(const TopoDS_Shape& shape);
    };
}

#include "solidObjCreator.tpp"

#endif //SOLIDOBJCREATOR_HPP
