#ifndef ASSISTPLNCYLSURFACEGERAOR_HPP
#define ASSISTPLNCYLSURFACEGERAOR_HPP

// McCAD
#include "inputconfig.hpp"
#include "cylSolid_impl.hpp"

namespace McCAD::Decomposition{
  class AssistPlnCylSurfaceGenerator{
  public:
      AssistPlnCylSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistPlnCylSurfaceGenerator();

      IO::InputConfig inputConfig;
      void operator()(Geometry::CYLSolid& solidObj);
  };
}

#endif //ASSISTPLNCYLSURFACEGERAOR_HPP
