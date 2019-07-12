#ifndef DECOMPOSESOLID_HPP
#define DECOMPOSESOLID_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
// OCC

namespace McCAD::Decomposition {
  class DecomposeSolid {
  private:
    class Impl;

  public:
    DecomposeSolid();
    ~DecomposeSolid();

    Impl* accessImpl() const;
    
  private:
    std::unique_ptr<Impl> pDecomposeSolidImpl;
  };
}
#endif //DECOMPOSESOLID_HPP
