#ifndef ASSISTCYLTORSURFACEGENERATOR_HPP
#define ASSISTCYLTORSURFACEGENERATOR_HPP

// C++
#include <optional>
#include <memory>
// McCAD
#include "inputconfig.hpp"
#include "mixedSolid_impl.hpp"
#include "boundSurface_impl.hpp"
// OCC
#include <Standard.hxx>

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
                          const Standard_Real& boxDiagonalLength,
                          const Standard_Real& meshDeflection);
      std::optional<std::shared_ptr<Geometry::BoundSurface>>
      generateThroughCurve(const std::shared_ptr<Geometry::BoundSurface>& firstFace,
                            const std::shared_ptr<Geometry::BoundSurface>& secondFace,
                            const std::shared_ptr<Geometry::Edge>& commonEdge,
                            const Standard_Real& boxDiagonalLength,
                            const Standard_Real& meshDeflection);
      std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
      generateThroughTwoLines(const std::shared_ptr<Geometry::BoundSurface>& firstFace,
                              const std::shared_ptr<Geometry::BoundSurface>& secondFace,
                              const std::shared_ptr<Geometry::Edge>& firstEdge,
                              const std::shared_ptr<Geometry::Edge>& secondEdge,
                              const Standard_Real& boxDiagonalLength,
                              const Standard_Real& meshDeflection);
  };
}

#endif //ASSISTCYLTORSURFACEGENERATOR_HPP
