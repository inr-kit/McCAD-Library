#ifndef STEPREADER_HPP
#define STEPREADER_HPP

// C++
#include <string>
#include <memory>
// McCAD
#include "inputdata.hpp"

namespace McCAD::IO{
  class STEPReader{
  private:
    class Impl;
    
  public:
    STEPReader(const std::string& fileName);
    STEPReader(const STEPReader& that);
    STEPReader(STEPReader&& that);
    ~STEPReader();

    STEPReader& operator=(const STEPReader& that);
    STEPReader& operator=(STEPReader&& that);

    McCAD::General::InputData getInputData() const;
    const std::string& getFileName() const;

  private:
    std::unique_ptr<Impl> pImpl;
  };
}

#endif //STEPREADER_HPP
