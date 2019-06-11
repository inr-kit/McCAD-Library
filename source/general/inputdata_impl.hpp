#ifndef INPUTDATAIMPL_HPP
#define INPUTDATAIMPL_HPP

// C++
#include <string>
// McCAD
#include "inputdata.hpp"
// OCC
#include "TCollection_AsciiString.hxx"
#include "TopTools_HSequenceOfShape.hxx"

namespace McCAD::General{
    class InputData::Impl {
    public:
      Impl() = default;
      Impl(const std::string& fileName);

      void setFileName(const std::string& fileName);
      const std::string& getFileName() const;
      void getInputSolidsList() const;
      
      std::string fileName;
      const Handle_TopTools_HSequenceOfShape inputSolidsList;

    private:
    };
  }

#endif //INPUTDATAIMPL_HPP
