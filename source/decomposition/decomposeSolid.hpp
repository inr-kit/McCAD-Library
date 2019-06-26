#ifndef DECOMPOSESOLID_HPP
#define DECOMPOSESOLID_HPP

// C++
#include <memory>
// McCAD
// OCC
#include <TopoDS_Solid.hxx>

namespace McCAD::Decomposition {
  class DecomposeSolid {
  private:
    class Impl;

  public:
    DecomposeSolid(const TopoDS_Solid& solid);
    ~DecomposeSolid();

    Impl* accessImpl() const;

  private:
    std::unique_ptr<Impl> pImpl;
  };
}
#endif //DECOMPOSESOLID_HPP
