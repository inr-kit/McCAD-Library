#ifndef ASSISTSURFACEGERAOR_HPP
#define ASSISTSURFACEGERAOR_HPP

// McCAD
#include "inputconfig.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "cylSolid_impl.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Decomposition{
  class AssistSurfaceGenerator{
  public:
      AssistSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistSurfaceGenerator();

      IO::InputConfig inputConfig;
      void operator()(Geometry::CYLSolid& solidObj);
      void operator()(Geometry::TORSolid& solidObj);
  };
}

#endif //ASSISTSURFACEGERAOR_HPP
