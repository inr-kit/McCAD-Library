#ifndef ASSISTPLNCYLSURFACEGERAOR_HPP
#define ASSISTPLNCYLSURFACEGERAOR_HPP

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
//OCC
#include <Standard.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class AssistPlnCylSurfaceGenerator{
  public:
      AssistPlnCylSurfaceGenerator(const IO::InputConfig& inputConfig);
      ~AssistPlnCylSurfaceGenerator();
  private:
      using edgesMap = std::map<Standard_Integer, std::vector<std::shared_ptr<Geometry::Edge>>>;
  public:
      IO::InputConfig inputConfig;
      void operator()(Geometry::CYLSolid& solidObj);
      Standard_Real getRadian(const TopoDS_Face& cylinder);
      std::optional<std::shared_ptr<Geometry::BoundSurface>>
      generateThroughLineAxis(const std::shared_ptr<Geometry::BoundSurface>& cylinderFace,
                              const std::shared_ptr<Geometry::Edge>& commonEdge,
                              const Standard_Real& boxDiagonalLength,
                              const Standard_Real& meshDeflection);
      std::optional<std::shared_ptr<Geometry::BoundSurface>>
      generateThroughTwoLines(const std::shared_ptr<Geometry::BoundSurface>& cylinderFace,
                              const std::shared_ptr<Geometry::Edge>& firstEdge,
                              const std::shared_ptr<Geometry::Edge>& secondEdge,
                              const Standard_Real& boxDiagonalLength,
                              const Standard_Real& meshDeflection);
  };
}

#endif //ASSISTPLNCYLSURFACEGERAOR_HPP
