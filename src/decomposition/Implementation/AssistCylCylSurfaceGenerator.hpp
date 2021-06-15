#ifndef ASSISTCYLCYLSURFACEGERAOR_HPP
#define ASSISTCYLCYLSURFACEGERAOR_HPP

// McCAD
#include "inputconfig.hpp"
#include "cylSolid_impl.hpp"

namespace McCAD::Decomposition{
  class AssistCylCylSurfaceGenerator{
  public:
      AssistCylCylSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistCylCylSurfaceGenerator();

      IO::InputConfig inputConfig;
      void operator()(Geometry::CYLSolid& solidObj);
  };
}

#endif //ASSISTCYLCYLSURFACEGERAOR_HPP
