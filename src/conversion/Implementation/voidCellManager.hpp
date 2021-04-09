#ifndef VOIDCELLMANAGER_HPP
#define VOIDCELLMANAGER_HPP

// C++
#include <string>
#include <vector>
#include <map>
// McCAD
#include "solid_impl.hpp"
#include "voidCell.hpp"
// OCC
#include <Standard.hxx>
#include <Bnd_Box.hxx>

namespace McCAD::Conversion{
    class VoidCellManager {
    public:
        VoidCellManager();
        ~VoidCellManager();
    private:
        // dimMap format: (SolidID, <AABB min, AABB center, AABB max>)
        using dimMap = std::map<Standard_Integer, std::tuple<Standard_Real,
                                                             Standard_Real,
                                                             Standard_Real>>;
        // surfaceTuple format: <cutting plane axis tag, position along the axis
        //                       number of intersections, number of expected splittings>
        using surfaceTuple = std::tuple<std::string, Standard_Real, Standard_Integer,
                                        Standard_Integer>;
        // membersMap format: (SolidID, <AABB, cutting plane axis tag,
        //                               position along the axis>)
        using membersMap = std::map<Standard_Integer, std::tuple<Bnd_Box,
                                                                 std::string,
                                                                 Standard_Real>>;
    public:
        dimMap xAxis, yAxis, zAxis;
        std::shared_ptr<VoidCell> voidCell;
        std::shared_ptr<VoidCell> operator()(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
                                             const Standard_Integer& maxSolidsPerVoidCell);
        std::shared_ptr<VoidCell> operator()(const membersMap& members, const Standard_Integer& maxSolidsPerVoidCell,
                                             const Standard_Integer& depth, const Standard_Integer& width);
        membersMap createLists(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
        void perform(const membersMap& members, const Standard_Integer& maxSolidsPerVoidCell);
        void populateLists(const membersMap& members);
        void updateVoidCell(const membersMap& members);
        std::pair<membersMap, membersMap> splitVoidCell(const surfaceTuple& surface,
                                                        const membersMap& members);
        std::pair<membersMap, membersMap> splitMembersList(const surfaceTuple& surface,
                                                           const membersMap& members,
                                                           const dimMap& axis);
        void updateBoundaries(const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
                              const std::shared_ptr<McCAD::Conversion::VoidCell>& voidCellDaughter,
                              const Standard_Integer& sense);
        Bnd_Box updateOverlapAABB(const Bnd_Box& aAABB,
                                  const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
                                  const Standard_Integer& sense);
    };
}
#endif //VOIDCELLMANAGER_HPP
