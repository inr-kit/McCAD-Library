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
      McCAD::General::InputData::Impl* accessImpl();
      
    };
  }

#endif //INPUTDATA_HPP
