#ifndef ASSISTSURFACEGERAOR_HPP
#define ASSISTSURFACEGERAOR_HPP

// McCAD
#include "solid_impl.hpp"
#include "torSolid_impl.hpp"
#include "cylSolid_impl.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Decomposition{
  class AssistSurfaceGenerator{
  public:
      void operator()(Geometry::CYLSolid& solidObj);
      void operator()(Geometry::TORSolid& solidObj,
                      Standard_Real angleTolerance = 0.785,
                      Standard_Real edgeTolerance = 1.0e-7); //45 degrees
  };
}

#endif //ASSISTSURFACEGERAOR_HPP
