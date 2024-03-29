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
// OCCT
#include <Bnd_Box.hxx>

namespace McCAD::Conversion{
    class VoidCellManager {
    public:
        VoidCellManager(const IO::InputConfig& inputConfig);
        VoidCellManager(const bool& BVHVoid,
                        const double& minVoidVolume,
                        const int& maxSolidsPerVoidCell,
                        const bool& voidGeneration,
                        const int& debugLevel);
        ~VoidCellManager();
    private:
        using solidsList = std::vector<std::shared_ptr<Geometry::Solid>>;
        // dimMap format: (SolidID, <AABB min, AABB center, AABB max>)
        using dimMap = std::map<int, std::tuple<double, double, double>>;
        // surfaceTuple format: <cutting plane axis tag, position along the axis,
        //                       number of intersections, number of expected splittings>
        using surfaceTuple = std::tuple<std::string, double, int, int>;
        // membersMap format: (SolidID, <AABB, cutting plane axis tag,
        //                               position along the axis>)
        using membersMap = std::map<int, std::tuple<Bnd_Box, std::string, double>>;
        // aabbTuple format: <AABB min, AABB max>
        using aabbTuple = std::tuple<double, double>;
        using aabbVec = std::vector<aabbTuple>;
    public:
        bool voidGeneration, BVHVoid;
        int maxSolidsPerVoidCell, debugLevel{0};
        double minVoidVolume;
        dimMap xAxis, yAxis, zAxis;
        std::shared_ptr<VoidCell> voidCell;
        std::shared_ptr<VoidCell> operator()(const solidsList& solidObjList);
        std::shared_ptr<VoidCell> operator()(const membersMap& members,
                                             const int& depth,
                                             const int& width,
                                             const std::string& key);
        std::shared_ptr<VoidCell> operator()(const membersMap& members,
                                             const int& depth,
                                             const int& width,
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
        aabbVec updateBoundaries(surfaceTuple& surface, const int& sense);
        Bnd_Box updateOverlapAABB(const Bnd_Box& aAABB, const surfaceTuple& surface,
                                  const int& sense);
    };
}
#endif //VOIDCELLMANAGER_HPP
