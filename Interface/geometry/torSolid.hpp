#ifndef TORSOLID_HPP
#define TORSOLID_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
#include "solid.hpp"
// OCC

namespace McCAD::Geometry{
  class TORSolid : public Solid{
  public:
    class Impl;

  public:
    TORSolid();
    ~TORSolid();

    Impl* accessTSImpl() const;
    
  private:
    std::unique_ptr<Impl> pTSImpl;
  };
}
#endif //TORSOLID_HPP
