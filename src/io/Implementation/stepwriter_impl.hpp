#ifndef STEPWRITER_IMPL_HPP
#define STEPWRITER_IMPL_HPP

// C++
#include <string>
#include <tuple>
#include <vector>
//McCAD
#include "stepwriter.hpp"
#include "inputconfig.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::IO{
  class STEPWriter::Impl {
  public:
      Impl(const IO::InputConfig& inputConfig);
      ~Impl();

      IO::InputConfig inputConfig;
  private:
    using solidsMap = std::vector<std::tuple<TCollection_ExtendedString,
                                             TopTools_HSequenceOfShape>>;
  public:
    void operator()(const solidsMap& shapes);
    std::string outputFileName;
  };
}

#endif //STEPWRITER_IMPL_HPP
