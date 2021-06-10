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
// OCC
#include <Standard.hxx>

namespace McCAD::Conversion{
    class SplitSurfaceSelector{
    public:
        SplitSurfaceSelector(const Standard_Integer& maxSolidsPerVoidCell,
                             const Standard_Real& minVoidVolume);
        ~SplitSurfaceSelector();
    private:
        // dimMap format: (SolidID, <AABB min, AABB center, AABB max>)
        using dimMap = std::map<Standard_Integer, std::tuple<Standard_Real,
                                                             Standard_Real,
                                                             Standard_Real>>;
        // centersDist format: <mu, std>
        using centersDist = std::vector<std::tuple<Standard_Real, Standard_Real>>;
        // centerTuple format: <AABB min, AABB center, AABB max>
        using centerTuple = std::tuple<Standard_Real, Standard_Real, Standard_Real>;
        // candidateTuple format: <solids to the left, position along axis,
        //                         solids to the right, intersections,
        //                         expected numbers of splittings>
        using candidateTuple = std::tuple<Standard_Integer, Standard_Real,
                                          Standard_Integer, Standard_Integer,
                                          Standard_Integer>;
        using candidateVec = std::vector<candidateTuple>;
        // surfaceTuple format: <cutting plane axis tag, position along the axis
        //                       number of intersections, number of expected splittings>
        using surfaceTuple = std::tuple<std::string, Standard_Real, Standard_Integer,
                                        Standard_Integer>;
        using surfaceVec = std::vector<surfaceTuple>;
    public:
        Standard_Integer maxSolidsPerVoidCell;
        Standard_Real minVoidVolume;
        Standard_Real xTolerance, yTolerance, zTolerance;
        std::optional<surfaceTuple> process(const dimMap& xMap,
                                            const dimMap& yMap,
                                            const dimMap& zMap,
                                            const std::shared_ptr<VoidCell>& voidCell);
        void calcDimTolerances(const std::shared_ptr<VoidCell>& voidCell);
        std::tuple<Standard_Real, Standard_Real> calcCentersParameters(const dimMap& aMap);
        std::optional<candidateTuple> selectAxisSplitSurface(
                const dimMap& aMap, const centerTuple& aabbList,
                const Standard_Real& dimTolerance);
        candidateTuple checkSplitSurfacePriority(candidateVec& candidates,
                                                 const dimMap& aMap);
    };
}

#endif //SPLITSURFACESELECTOR_HPP
