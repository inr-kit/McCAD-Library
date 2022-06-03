#ifndef CYLSOLID_HPP
#define CYLSOLID_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
#include "solid.hpp"
// OCC

namespace McCAD::Geometry{
  class CYLSolid : public Solid{
  public:
    class Impl;

  public:
    CYLSolid();
    ~CYLSolid();

    Impl* accessCSImpl() const;
    
  private:
    std::unique_ptr<Impl> pCSImpl;
  };
}
#endif //CYLSOLID_HPP
