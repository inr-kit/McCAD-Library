#ifndef INPUTDATAIMPL_HPP
#define INPUTDATAIMPL_HPP

// C++
#include <string>
// McCAD
#include "inputdata.hpp"
// OCC
//#include <TCollection_AsciiString.hxx>
//#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::General{
    class InputData::Impl {
    public:
        Impl() = default;
        Impl(const std::string& fileName);

        void setFileName(const std::string& fileName); //TCollection_AsciiString & FileName);
        const std::string& getFileName() const;
    private:
        std::string fileName; //TCollection_AsciiString InputFileName;
    };
  }

#endif //INPUTDATAIMPL_HPP
