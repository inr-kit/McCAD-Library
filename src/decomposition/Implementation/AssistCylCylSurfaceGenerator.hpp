#ifndef ASSISTCYLCYLSURFACEGENERATOR_HPP
#define ASSISTCYLCYLSURFACEGENERATOR_HPP

// C++
#include <map>
#include <vector>
#include <memory>
#include <optional>
// McCAD
#include "inputconfig.hpp"
#include "edge_impl.hpp"
#include "cylSolid_impl.hpp"
#include "boundSurface_impl.hpp"

namespace McCAD::Decomposition{
  class AssistCylCylSurfaceGenerator{
  public:
      AssistCylCylSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistCylCylSurfaceGenerator();
  private:
      using edgesMap = std::map<int, std::vector<std::shared_ptr<Geometry::Edge>>>;
  public:
      IO::InputConfig inputConfig;
      void operator()(Geometry::CYLSolid& solidObj);
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
                              const std::shared_ptr<Geometry::Edge>& firstEdge,
                              const std::shared_ptr<Geometry::Edge>& secondEdge,
                              const double& boxDiagonalLength,
                              const double& meshDeflection);
  };
}

#endif //ASSISTCYLCYLSURFACEGERAOR_HPP
