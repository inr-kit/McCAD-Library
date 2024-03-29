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

    std::string fileName;
    void readSTEP();
    bool getLabelInfo(const TDF_Label& aLabel);
    bool iterateLabelChilds(const TDF_Label& aLabel);
    bool basicReader(const std::string& fileName);
    void writeVolumes();
    void writeSurfacesTally();
  };
}

#endif //STEPREADER_IMPL_HPP
