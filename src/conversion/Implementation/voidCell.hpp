#ifndef VOIDCELL_HPP
#define VOIDCELL_HPP

// C++
#include <vector>
#include <memory>
#include <map>
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
      // centerTuple format: <AABB min, AABB center, AABB max>
      using centerTuple = std::tuple<Standard_Real, Standard_Real, Standard_Real>;
      // membersMap format: (SolidID, <AABB, cutting plane axis tag,
      //                               position along the axis>)
      using membersMap = std::map<Standard_Integer, std::tuple<Bnd_Box,
                                                               std::string,
                                                               Standard_Real>>;
      // updateTuple format: <AABB min, AABB max>
      using updateTuple = std::tuple<Standard_Real, Standard_Real>;
  public:
      Bnd_Box aabb;
      TopoDS_Solid aabbSolid;
      Standard_Boolean splitted{Standard_False};
      Standard_Integer depth, width;
      std::vector<Standard_Integer> solidIDList;
      std::vector<std::shared_ptr<VoidCell>> daughterVoidCells;
      Standard_Real minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0};
      centerTuple xAxis, yAxis, zAxis;
      updateTuple xAxisUpdate, yAxisUpdate, zAxisUpdate;
      std::string voidExpr, voidSurfExpr;
      Standard_Integer voidSurfNumber;

      void addSolidIDs(const membersMap& members);
      void addSolids(const membersMap& members);
      void updateAABB();
      Standard_Real getAABBVolume();
      void outputAABB();
  };
}

#endif //VOIDCELL_HPP
