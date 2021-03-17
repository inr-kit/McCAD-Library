#ifndef STEPREADER_IMPL_HPP
#define STEPREADER_IMPL_HPP

// C++
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
// McCAD
#include "stepreader.hpp"
// OCC
#include <TDF_Label.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::IO{
  class STEPReader::Impl{
  public:
    Impl(const std::string& fileName);
    ~Impl();

    std::string fileName;
    std::shared_ptr<TopTools_HSequenceOfShape> sequenceOfShape;
    std::vector<TCollection_ExtendedString> shapeNames = {};
    std::vector<std::tuple<TopoDS_Shape, TCollection_ExtendedString>> shapesInfoList;

    void readSTEP();
    void getLabelInfo(const TDF_Label& aLabel);
    bool iterateLabelChilds(const TDF_Label& aLabel,
                            const TCollection_ExtendedString& aName);
  };
}

#endif //STEPREADER_IMPL_HPP
