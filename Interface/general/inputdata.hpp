#ifndef INPUTDATA_HPP
#define INPUTDATA_HPP

// C++
#include <string>
#include <memory>

namespace McCAD::General{
  class InputData{
  private:
    class Impl;

  public:
    InputData();
    InputData(const InputData& that);
    InputData(InputData&& that);
    ~InputData();
    
    InputData& operator=(const InputData& that);
    InputData& operator=(InputData&& that);
    int getSize();

    Impl* accessImpl() const;
    
  private:
    std::unique_ptr<Impl> pImpl;
  };
}

#endif //INPUTDATA_HPP
