#ifndef INPUTDATA_HPP
#define INPUTDATA_HPP

// C++
#include <string>
#include <memory>
// McCAD

namespace McCAD::General {
    class InputData {
    public:
      InputData();
      ~InputData();
      InputData(const std::string& fileName);

      void setFileName(const std::string& fileName);
      const std::string& getFileName() const;
      void getInputSolidsList() const;

    private:
      class Impl;
      std::unique_ptr<Impl> pImpl;

    public:
      // Allow access to Impl class from other classes.
      const McCAD::General::InputData::Impl* accessImpl() const;
      
    };
  }

#endif //INPUTDATA_HPP
