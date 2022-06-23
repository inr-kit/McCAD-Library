#ifndef DECOMPOSESOLID_HPP
#define DECOMPOSESOLID_HPP

// C++
#include <memory>
// McCAD
#include "inputconfig.hpp"
// OCC

namespace McCAD::Decomposition {
  class DecomposeSolid {
  private:
    class Impl;

  public:
    DecomposeSolid(const IO::InputConfig& inputConfig);
    ~DecomposeSolid();

    Impl* accessDSImpl() const;
    
  private:
    std::unique_ptr<Impl> pDSImpl;
  };
}
#endif //DECOMPOSESOLID_HPP
