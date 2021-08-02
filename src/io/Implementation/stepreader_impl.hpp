#ifndef STEPREADER_IMPL_HPP
#define STEPREADER_IMPL_HPP

// C++
#include <string>
#include <vector>
#include <memory>
#include <tuple>
// McCAD
#include "stepreader.hpp"
#include "inputconfig.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TCollection_ExtendedString.hxx>
#include <STEPControl_Reader.hxx>

namespace McCAD::IO{
  class STEPReader::Impl{
  public:
    Impl(const IO::InputConfig& inputConfig);
    ~Impl();

    IO::InputConfig inputConfig;
    std::shared_ptr<TopTools_HSequenceOfShape> sequenceOfShape;
    std::vector<TCollection_ExtendedString> shapeNames;
    std::vector<std::tuple<TopoDS_Shape, TCollection_ExtendedString>> shapesInfoMap;
    //std::vector<std::tuple<TopoDS_Shape, TCollection_ExtendedString,
    //                       TCollection_ExtendedString>> shapesInfoMap;

    void readSTEP();
    Standard_Boolean getLabelInfo(const TDF_Label& aLabel);
    Standard_Boolean iterateLabelChilds(const TDF_Label& aLabel);
    Standard_Boolean basicReader(const std::string& fileName);
  };
}

#endif //STEPREADER_IMPL_HPP
