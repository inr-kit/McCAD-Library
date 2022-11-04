#ifndef ASSISTPLNCONESURFACEGERAOR_HPP
#define ASSISTPLNCONESURFACEGERAOR_HPP

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
//OCC
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

  };
}

#endif //ASSISTPLNCONESURFACEGERAOR_HPP
