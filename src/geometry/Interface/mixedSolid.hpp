#ifndef MXDSOLID_HPP
#define MXDSOLID_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
#include "solid.hpp"
// OCC

namespace McCAD::Geometry{
  class MXDSolid : public Solid{
  public:
      class Impl;
      MXDSolid();
      ~MXDSolid();

      Impl* accessXSImpl() const;
  private:
      std::unique_ptr<Impl> pXSImpl;
  };
}
#endif //MXDSOLID_HPP
