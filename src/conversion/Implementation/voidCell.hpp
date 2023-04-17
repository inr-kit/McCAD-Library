#ifndef VOIDCELL_HPP
#define VOIDCELL_HPP

// C++
#include <vector>
#include <memory>
#include <map>
// McCAD
#include "solid_impl.hpp"
// OCCT
#include <Bnd_Box.hxx>
#include <TopoDS_Solid.hxx>

namespace McCAD::Conversion{
  class VoidCell{
  public:
      VoidCell();
      VoidCell(const int& depth, const int& width, const std::string& key);
      ~VoidCell();
  private :
      // centerTuple format: <AABB min, AABB center, AABB max>
      using centerTuple = std::tuple<double, double, double>;
      // membersMap format: (SolidID, <AABB, cutting plane axis tag,
      //                               position along the axis>)
      using membersMap = std::map<int, std::tuple<Bnd_Box, std::string, double>>;
      // updateTuple format: <AABB min, AABB max>
      using updateTuple = std::tuple<double, double>;
  public:
      Bnd_Box aabb;
      TopoDS_Solid aabbSolid;
      bool splitted{Standard_False};
      int depth, width, voidSurfNumber, MCCellID{0};
      std::string key, voidExpr, voidSurfExpr;
      //using for mcx writing
      std::tuple<std::string, std::string, std::string> voidParMap;
      std::vector<int> solidIDList;
      std::vector<std::shared_ptr<VoidCell>> daughterVoidCells;
      double minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0};
      centerTuple xAxis, yAxis, zAxis;
      updateTuple xAxisUpdate, yAxisUpdate, zAxisUpdate;

      void addSolidIDs(const membersMap& members);
      void addSolids(const membersMap& members);
      void updateAABB();
      double getAABBVolume();
      void outputAABB();
  };
}

#endif //VOIDCELL_HPP
