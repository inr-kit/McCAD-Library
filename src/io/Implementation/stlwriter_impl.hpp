#ifndef STLWRITER_IMPL_HPP
#define STLWRITER_IMPL_HPP

// C++
#include <string>
#include <filesystem>
//McCAD
#include "stlwriter.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::IO{
  class STLWriter::Impl {
  public:
      Impl(const std::string& fileName);
      ~Impl();

      void operator()(const Handle_TopTools_HSequenceOfShape& shapes);
      std::string conversionfileName;
  };
}

#endif //STLWRITER_IMPL_HPP
