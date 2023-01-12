#ifndef ASSISTSURFACEGENERATOR_HPP
#define ASSISTSURFACEGENERATOR_HPP

// McCAD
#include "inputconfig.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "mixedSolid_impl.hpp"
#include "conSolid_impl.hpp"

namespace McCAD::Decomposition{
  class AssistSurfaceGenerator{
  public:
      AssistSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistSurfaceGenerator();

      IO::InputConfig inputConfig;
      void operator()(Geometry::CYLSolid& solidObj);
      void operator()(Geometry::TORSolid& solidObj);
      void operator()(Geometry::MXDSolid& solidObj);
      bool checkFillet(Geometry::CYLSolid& solidObj);
      void operator()(Geometry::CONSolid& solidObj);
  };
}

#endif //ASSISTSURFACEGENERATOR_HPP
