#ifndef STEPREADER_HPP
#define STEPREADER_HPP

// C++
#include <string>
#include <memory>
// McCAD
#include "inputdata.hpp"
#include "inputconfig.hpp"

namespace McCAD::IO{
  class STEPReader{
  private:
    class Impl;
    
  public:
    STEPReader(const IO::InputConfig& inputConfig);
    STEPReader(const STEPReader& that);
    STEPReader(STEPReader&& that);
    ~STEPReader();

    STEPReader& operator=(const STEPReader& that);
    STEPReader& operator=(STEPReader&& that);

    General::InputData getInputData() const;

  private:
    std::unique_ptr<Impl> pImpl;
  };
}

#endif //STEPREADER_HPP
