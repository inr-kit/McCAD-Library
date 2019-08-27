#ifndef STEPWRITER_IMPL_HPP
#define STEPWRITER_IMPL_HPP

// C++
#include <string>
#include <filesystem>
//McCAD
#include "stepwriter.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <STEPControl_Writer.hxx>

namespace McCAD::IO{
  class STEPWriter::Impl {
  public:
    Impl(const std::string& fileName);
    void operator()(const Handle_TopTools_HSequenceOfShape& shapes);
    std::string outputfileName;
  };
}

#endif //STEPWRITER_IMPL_HPP
