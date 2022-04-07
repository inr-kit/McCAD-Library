#ifndef SPLITSURFACESELECTOR_HPP
#define SPLITSURFACESELECTOR_HPP

// C++
#include <tuple>
#include <vector>
#include <memory>
#include <map>
#include <optional>
// McCAD
#include <voidCell.hpp>

namespace McCAD::Conversion{
    class SplitSurfaceSelector{
    public:
        SplitSurfaceSelector(const int& maxSolidsPerVoidCell,
                             const double& minVoidVolume);
        ~SplitSurfaceSelector();
    private:
        // dimMap format: (SolidID, <AABB min, AABB center, AABB max>)
        using dimMap = std::map<int, std::tuple<double, double, double>>;
        // centersDist format: <mu, std>
        using centersDist = std::vector<std::tuple<double, double>>;
        // centerTuple format: <AABB min, AABB center, AABB max>
        using centerTuple = std::tuple<double, double, double>;
        // candidateTuple format: <solids to the left, position along axis,
        //                         solids to the right, intersections,
        //                         expected numbers of splittings>
        using candidateTuple = std::tuple<int, double, int, int, int>;
        using candidateVec = std::vector<candidateTuple>;
        // surfaceTuple format: <cutting plane axis tag, position along the axis
        //                       number of intersections, number of expected splittings>
        using surfaceTuple = std::tuple<std::string, double, int, int>;
        using surfaceVec = std::vector<surfaceTuple>;
    public:
        int maxSolidsPerVoidCell;
        double minVoidVolume, xTolerance, yTolerance, zTolerance;
        std::optional<surfaceTuple> process(const dimMap& xMap,
                                            const dimMap& yMap,
                                            const dimMap& zMap,
                                            const std::shared_ptr<VoidCell>& voidCell);
        void calcDimTolerances(const std::shared_ptr<VoidCell>& voidCell);
        std::tuple<double, double> calcCentersParameters(const dimMap& aMap);
        std::optional<candidateTuple> selectAxisSplitSurface(
                const dimMap& aMap, const centerTuple& aabbList,
                const double& dimTolerance);
        candidateTuple checkSplitSurfacePriority(candidateVec& candidates,
                                                 const dimMap& aMap);
    };
}

#endif //SPLITSURFACESELECTOR_HPP
