#ifndef INPUTDATA_IMPL_HPP
#define INPUTDATA_IMPL_HPP

// C++
#include <string>
#include <memory>
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

    std::vector<std::tuple<TopoDS_Shape, TCollection_ExtendedString>> zinputSolidsList;
    std::shared_ptr<TopTools_HSequenceOfShape> inputSolidsList;
    int size{0};

    void updateSize();
  };
}

#endif //INPUTDATA_IMPL_HPP
