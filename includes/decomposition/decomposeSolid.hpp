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

    Standard_Real meshDeflection;
    Standard_Real boxSquareLength;
    Standard_Integer recurrenceDepth = 0;
    Standard_Boolean splitSurface = Standard_False;
    
  private:
    std::unique_ptr<Impl> pImpl;
  };
}
#endif //DECOMPOSESOLID_HPP
