#ifndef STEPWRITER_HPP
#define STEPWRITER_HPP

// C++
#include <string>
#include <memory>
// McCAD
#include "inputdata.hpp"

namespace McCAD::IO{
  class STEPWriter {
  private:
    class Impl;

  public:
    STEPWriter(const std::string& fileName, const General::InputData& inputData);
    STEPWriter(const STEPWriter& that);
    STEPWriter(STEPWriter&& that);
    ~STEPWriter();

    STEPWriter& operator=(const STEPWriter& that);
    STEPWriter& operator=(STEPWriter&& that);

    const std::string& getFileName() const;

  private:
    std::unique_ptr<Impl> pImpl;
  };
}

#endif //STEPWRITER_HPP
