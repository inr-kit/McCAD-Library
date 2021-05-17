#ifndef STEPWRITER_IMPL_HPP
#define STEPWRITER_IMPL_HPP

// C++
#include <string>
#include <filesystem>
#include <vector>
//McCAD
#include "stepwriter.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::IO{
  class STEPWriter::Impl {
  public:
    Impl(const std::string& fileName);
  private:
    using solidsMap = std::vector<std::tuple<TCollection_ExtendedString,
                                             TopTools_HSequenceOfShape>>;
  public:
    void operator()(const solidsMap& shapes);
    std::string outputfileName;
  };
}

#endif //STEPWRITER_IMPL_HPP
