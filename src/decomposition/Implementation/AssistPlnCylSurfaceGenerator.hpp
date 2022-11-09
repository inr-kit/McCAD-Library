#ifndef ASSISTPLNCYLSURFACEGENERATOR_HPP
#define ASSISTPLNCYLSURFACEGENERATOR_HPP

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
// OCCT
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class AssistPlnCylSurfaceGenerator{
  public:
      AssistPlnCylSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistPlnCylSurfaceGenerator();
  private:
      using edgesMap = std::map<int, std::vector<std::shared_ptr<Geometry::Edge>>>;
  public:
      IO::InputConfig inputConfig;
      void operator()(Geometry::CYLSolid& solidObj);
      std::optional<std::shared_ptr<Geometry::BoundSurface>>
      generateThroughLineAxis(const std::shared_ptr<Geometry::BoundSurface>& cylinderFace,
                              const std::shared_ptr<Geometry::Edge>& commonEdge,
                              const double& boxDiagonalLength,
                              const double& meshDeflection);
      std::optional<std::shared_ptr<Geometry::BoundSurface>>
      generateThroughTwoLines(const std::shared_ptr<Geometry::BoundSurface>& cylinderFace,
                              const std::shared_ptr<Geometry::Edge>& firstEdge,
                              const std::shared_ptr<Geometry::Edge>& secondEdge,
                              const double& boxDiagonalLength,
                              const double& meshDeflection);
  };
}

#endif //ASSISTPLNCYLSURFACEGENERATOR_HPP
