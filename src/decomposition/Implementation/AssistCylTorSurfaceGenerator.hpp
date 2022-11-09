#ifndef ASSISTCYLTORSURFACEGENERATOR_HPP
#define ASSISTCYLTORSURFACEGENERATOR_HPP

// C++
#include <optional>
#include <memory>
// McCAD
#include "inputconfig.hpp"
#include "mixedSolid_impl.hpp"
#include "boundSurface_impl.hpp"

namespace McCAD::Decomposition{
  class AssistCylTorSurfaceGenerator{
  public:
      AssistCylTorSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistCylTorSurfaceGenerator();

      IO::InputConfig inputConfig;
      void operator()(Geometry::MXDSolid& solidObj);
      std::optional<std::shared_ptr<Geometry::BoundSurface>>
      generateThroughLine(const std::shared_ptr<Geometry::BoundSurface>& firstFace,
                          const std::shared_ptr<Geometry::BoundSurface>& secondFace,
                          const std::shared_ptr<Geometry::Edge>& commonEdge,
                          const double& boxDiagonalLength,
                          const double& meshDeflection);
      std::optional<std::shared_ptr<Geometry::BoundSurface>>
      generateThroughCurve(const std::shared_ptr<Geometry::BoundSurface>& firstFace,
                            const std::shared_ptr<Geometry::BoundSurface>& secondFace,
                            const std::shared_ptr<Geometry::Edge>& commonEdge,
                            const double& boxDiagonalLength,
                            const double& meshDeflection);
      std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
      generateThroughTwoLines(const std::shared_ptr<Geometry::BoundSurface>& firstFace,
                              const std::shared_ptr<Geometry::BoundSurface>& secondFace,
                              const std::shared_ptr<Geometry::Edge>& firstEdge,
                              const std::shared_ptr<Geometry::Edge>& secondEdge,
                              const double& boxDiagonalLength,
                              const double& meshDeflection);
  };
}

#endif //ASSISTCYLTORSURFACEGENERATOR_HPP
