#ifndef DECOMPOSESOLID_HPP
#define DECOMPOSESOLID_HPP

// C++
#include <memory>
// McCAD
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
    std::unique_ptr<Impl> pImpl;
  };
}
#endif //DECOMPOSESOLID_HPP
