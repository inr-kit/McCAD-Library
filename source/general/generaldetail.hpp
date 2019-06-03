#ifndef GENERALDETAIL_HPP
#define GENERALDETAIL_HPP

// C++
#include <string>
// McCAD

// OCC imports
//#include <TCollection_AsciiString.hxx>
//#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD { namespace General { namespace Detail {
    class InputData_Impl {
    public:
      InputData_Impl() = default;
    private:
      std::string InputFileName; //TCollection_AsciiString InputFileName;
      void SetFileName(std::string & FileName); //TCollection_AsciiString & FileName);
    };
    }}}

#endif //GENERALDetail_HPP
