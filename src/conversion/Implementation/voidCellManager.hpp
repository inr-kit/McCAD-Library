#ifndef VOIDCELLMANAGER_HPP
#define VOIDCELLMANAGER_HPP

// C++
#include <string>
#include <vector>
#include <tuple>
#include <map>
// McCAD
#include "solid_impl.hpp"
#include "inputconfig.hpp"
#include "voidCell.hpp"
// OCC
#include <Standard.hxx>
#include <Bnd_Box.hxx>

namespace McCAD::Conversion{
    class VoidCellManager {
    public:
        VoidCellManager(const IO::InputConfig& inputConfig);
        VoidCellManager(const Standard_Boolean& BVHVoid,
                        const Standard_Real& minVoidVolume,
                        const Standard_Integer& maxSolidsPerVoidCell,
                        const Standard_Boolean& voidGeneration);
        ~VoidCellManager();
    private:
        using solidsList = std::vector<std::shared_ptr<Geometry::Solid>>;
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
        // aabbTuple format: <AABB min, AABB max>
        using aabbTuple = std::tuple<Standard_Real, Standard_Real>;
        using aabbVec = std::vector<aabbTuple>;
    public:
        Standard_Boolean voidGeneration;
        Standard_Boolean BVHVoid;
        Standard_Integer maxSolidsPerVoidCell;
        Standard_Real minVoidVolume;
        dimMap xAxis, yAxis, zAxis;
        std::shared_ptr<VoidCell> voidCell;
        std::shared_ptr<VoidCell> operator()(const solidsList& solidObjList);
        std::shared_ptr<VoidCell> operator()(const membersMap& members,
                                             const Standard_Integer& depth,
                                             const Standard_Integer& width,
                                             const std::string& key);
        std::shared_ptr<VoidCell> operator()(const membersMap& members,
                                             const Standard_Integer& depth,
                                             const Standard_Integer& width,
                                             const std::string& key,
                                             const aabbTuple& xAxisAABB,
                                             const aabbTuple& yAxisAABB,
                                             const aabbTuple& zAxisAABB);
        membersMap createLists(const solidsList& solidObjList);
        void perform(const membersMap& members);
        void populateLists(const membersMap& members);
        void updateVoidCell(const membersMap& members);
        std::pair<membersMap, membersMap> splitVoidCell(const surfaceTuple& surface,
                                                        const membersMap& members);
        std::pair<membersMap, membersMap> splitMembersList(const surfaceTuple& surface,
                                                           const membersMap& members,
                                                           const dimMap& axis);
        aabbVec updateBoundaries(surfaceTuple& surface, const Standard_Integer& sense);
        Bnd_Box updateOverlapAABB(const Bnd_Box& aAABB, const surfaceTuple& surface,
                                  const Standard_Integer& sense);
    };
}
#endif //VOIDCELLMANAGER_HPP
