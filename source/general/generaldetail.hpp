#ifndef GENERALDETAIL_HPP
#define GENERALDETAIL_HPP

// C++
#include <string>
#include "general.hpp"
// McCAD

// OCC imports
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

#endif //GENERALDetail_HPP
