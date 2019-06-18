#ifndef INPUTDATAIMPL_HPP
#define INPUTDATAIMPL_HPP

// C++
// McCAD
#include "inputdata.hpp"
// OCC
#include "Handle_TopTools_HSequenceOfShape.hxx"
#include "TCollection_AsciiString.hxx"
#include "TopTools_HSequenceOfShape.hxx"

namespace McCAD::General{
    class InputData::Impl {
    public:
      Impl() = default;
      Impl(const std::string& fileName);

      std::string fileName;
      Handle_TopTools_HSequenceOfShape inputSolidsList;
      
      void setFileName(const std::string& fileName);
      const std::string& getFileName();
      void getInputSolidsList();
      
    private:
    };
  }

#endif //INPUTDATAIMPL_HPP
