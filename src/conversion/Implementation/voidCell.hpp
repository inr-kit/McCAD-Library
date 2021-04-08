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
      VoidCell(const Standard_Integer& depth, const Standard_Integer& width);
      ~VoidCell();
  private :
      using centerTuple = std::tuple<Standard_Real, Standard_Real, Standard_Real>;
  public:
      Bnd_Box aabb;
      TopoDS_Solid aabbSolid;
      Standard_Boolean split{Standard_False};
      Standard_Integer depth, width;
      std::vector<Standard_Integer> solidIDList;
      std::vector<std::shared_ptr<VoidCell>> daughterVoidCells;
      Standard_Real minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0};
      centerTuple xAxis, yAxis, zAxis;

      void addSolidIDs(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
      void addSolids(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
  };
}

#endif //VOIDCELL_HPP
