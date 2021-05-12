#ifndef STEPREADER_IMPL_HPP
#define STEPREADER_IMPL_HPP

// C++
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
// McCAD
#include "stepreader.hpp"
// OCC
#include <TDF_Label.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::IO{
  class STEPReader::Impl{
  public:
    Impl(const std::string& fileName);

    void readSTEP();
    void getLabelInfo(const TDF_Label& aLabel);
    bool iterateLabelChilds(const TDF_Label& aLabel,
                            const TCollection_ExtendedString& aName);

    std::string fileName;
    Handle_TopTools_HSequenceOfShape sequenceOfShape;
    std::vector<TCollection_ExtendedString> shapeNames = {};
  };
}

#endif //STEPREADER_IMPL_HPP
