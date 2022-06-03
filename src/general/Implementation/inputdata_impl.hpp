#ifndef INPUTDATA_IMPL_HPP
#define INPUTDATA_IMPL_HPP

// C++
#include <vector>
#include <tuple>
// McCAD
#include "inputdata.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TCollection_ExtendedString.hxx>
#include <Standard_Transient.hxx>

namespace McCAD::General{
    class InputData::Impl{
    public:
        Impl();
        ~Impl();
    private:
        using shapeTuple = std::tuple<TopoDS_Shape, TCollection_ExtendedString>;
        using shapesMap = std::vector<shapeTuple>;
        using solidsMap = std::vector<std::tuple<TCollection_ExtendedString,
                                                 TopTools_HSequenceOfShape>>;
    public:
        shapesMap inputShapesMap;
        solidsMap outputShapesMap;
        int size{0};
        void updateSize();
  };
}

#endif //INPUTDATA_IMPL_HPP
