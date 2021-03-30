#ifndef VOIDCELL_HPP
#define VOIDCELL_HPP

// C++
#include <vector>
#include <memory>
// McCAD
#include "solid_impl.hpp"
// OCC
#include <Standard.hxx>
#include <Bnd_Box.hxx>
#include <TopoDS_Solid.hxx>

namespace McCAD::Conversion{
  class VoidCell{
  public:
      VoidCell();
      VoidCell(Standard_Integer& depth, Standard_Integer& width);
      ~VoidCell();
  private:
      Standard_Integer depth, width;
      std::vector<std::shared_ptr<Geometry::Solid>> solidObjList;
      std::vector<Standard_Integer> solidIDList;
      Bnd_Box aabb;
      TopoDS_Solid aabbSolid;
      Standard_Real minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0}, center{0};
  public:
      void addSolids(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
  };
}

#endif //VOIDCELL_HPP
