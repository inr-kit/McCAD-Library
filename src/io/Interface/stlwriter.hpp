#ifndef STLWRITER_HPP
#define STLWRITER_HPP

// C++
#include <string>
// McCAD
#include "inputdata.hpp"

namespace McCAD::IO{
  class STLWriter {
  private:
    class Impl;

  public:
    STLWriter(const std::string& fileName, const General::InputData& inputData);
    ~STLWriter();

  private:
    std::unique_ptr<Impl> pImpl;
  };
}

#endif //STLWRITER_HPP
