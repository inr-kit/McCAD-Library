#ifndef INPUTCONFIG_HPP
#define INPUTCONFIG_HPP

//C++
#include <string>

namespace McCAD::IO{
  class InputConfig{
  public:
    InputConfig(std::string& currentPath);
    writeTemplate(std::string& currentPath);
  };
}

#endif // INPUTCONFIG_H
