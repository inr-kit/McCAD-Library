#ifndef STEPWRITER_IMPL_HPP
#define STEPWRITER_IMPL_HPP

// C++
#include <string>
#include <filesystem>
#include <memory>
//McCAD
#include "stepwriter.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::IO{
  class STEPWriter::Impl {
  public:
    Impl(const std::string& fileName);
    void operator()(const std::shared_ptr<TopTools_HSequenceOfShape>& shapes);
    std::string outputfileName;
  };
}

#endif //STEPWRITER_IMPL_HPP
