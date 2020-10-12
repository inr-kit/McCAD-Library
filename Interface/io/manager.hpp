#ifndef MANAGER_HPP
#define MANAGER_HPP

// C++
//McCAD
#include "Inputconfig.hpp"

namespace McCAD::IO{
  class Manager{
  public:
      Manager(InputConfig& inpufConfig);
      bool decompose{False}, convert{False}, decompose_and_convert{False};
  };
}

#endif //MANAGER_HPP
