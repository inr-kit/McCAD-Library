#ifndef STEPREADER_IMPL_HPP
#define STEPREADER_IMPL_HPP

// C++
#include <string>
#include <vector>
#include <memory>
#include <tuple>
// McCAD
#include "stepreader.hpp"
// OCC
#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::IO{
  class STEPReader::Impl{
  public:
    Impl(const std::string& fileName);
    ~Impl();

    std::string fileName;
    std::shared_ptr<TopTools_HSequenceOfShape> sequenceOfShape;
    std::vector<TCollection_ExtendedString> shapeNames;
    std::vector<std::tuple<TopoDS_Shape, TCollection_ExtendedString>> shapesInfoMap;

    void readSTEP();
    void getLabelInfo(const TDF_Label& aLabel);
    bool iterateLabelChilds(const TDF_Label& aLabel,
                            const TCollection_ExtendedString& aName);
  };
}

#endif //STEPREADER_IMPL_HPP
