// C++
#include <iostream>
#include <string>
// McCAD
#include "McCADConfig.hpp"
#include "general.hpp"
#include "decomposition.hpp"

int main () {
  std::string FileName = "SomeFileName";
  McCAD::General::InputData InputDD;
  InputDD.InputD->SetFileName(FileName);
  std::cout << "Loading file: " << FileName << std::endl;
  return 0;
}
