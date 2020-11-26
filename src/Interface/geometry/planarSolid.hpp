#ifndef PLSOLID_HPP
#define PLSOLID_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
#include "solid.hpp"
// OCC

namespace McCAD::Geometry{
  class PLSolid : public Solid{
  public:
    class Impl;

  public:
    PLSolid();
    ~PLSolid();

    Impl* accessPSImpl() const;
    
  private:
    std::unique_ptr<Impl> pPSImpl;
  };
}
#endif //PLSOLID_HPP
