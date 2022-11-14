#ifndef ASSISTPLNCONESURFACEGENERATOR_HPP
#define ASSISTPLNCONESURFACEGENERATOR_HPP

// C++
#include <map>
#include <vector>
#include <memory>
#include <optional>
// McCAD
#include "inputconfig.hpp"
#include "edge_impl.hpp"
#include "conSolid_impl.hpp"
#include "boundSurface_impl.hpp"
// OCCT
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class AssistPlnConeSurfaceGenerator{
  public:
      AssistPlnConeSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistPlnConeSurfaceGenerator();
  private:
      using edgesMap = std::map<int, std::vector<std::shared_ptr<Geometry::Edge>>>;
  public:
      IO::InputConfig inputConfig;
      void operator()(Geometry::CONSolid& solidObj);
      std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
      generateThroughLineAxis(const std::shared_ptr<Geometry::BoundSurface>& coneSurface,
                              const std::shared_ptr<Geometry::Edge>& commonEdge,
                              const double& boxDiagonalLength, const double& meshDeflection);
      std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
      generateThroughTwoLines(const std::shared_ptr<Geometry::BoundSurface>& coneSurface,
                              const std::shared_ptr<Geometry::Edge>& firstEdge,
                              const std::shared_ptr<Geometry::Edge>& secondEdge,
                              const double& boxDiagonalLength, const double& meshDeflection);
  };
}

#endif //ASSISTPLNCONESURFACEGENERATOR_HPP
