#ifndef STEPWRITER_HPP
#define STEPWRITER_HPP

// C++
#include <string>
#include <memory>
// McCAD
#include "inputdata.hpp"
#include "inputconfig.hpp"

namespace McCAD::IO{
  class STEPWriter {
  private:
    class Impl;

  public:
    STEPWriter(const IO::InputConfig& inputConfig, const General::InputData& outputData);
    STEPWriter(const STEPWriter& that);
    STEPWriter(STEPWriter&& that);
    ~STEPWriter();

    STEPWriter& operator=(const STEPWriter& that);
    STEPWriter& operator=(STEPWriter&& that);

  private:
    std::unique_ptr<Impl> pImpl;
  };
}

#endif //STEPWRITER_HPP
