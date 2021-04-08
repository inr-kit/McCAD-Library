#ifndef SPLITSURFACESELECTOR_HPP
#define SPLITSURFACESELECTOR_HPP

// C++
#include <tuple>
#include <vector>
#include <memory>
#include <map>
// McCAD
#include <voidCell.hpp>
// OCC
#include <Standard.hxx>

namespace McCAD::Conversion{
    class SplitSurfaceSelector{
    public:
        SplitSurfaceSelector(const Standard_Integer& maxSolidsPerVoidCell);
        ~SplitSurfaceSelector();
    private:
        using dimMap = std::map<Standard_Integer, std::tuple<Standard_Real,
                                                             Standard_Real,
                                                             Standard_Real>>;
        using centersDist = std::vector<std::tuple<Standard_Real, Standard_Real>>;
        using centerTuple = std::tuple<Standard_Real, Standard_Real, Standard_Real>;
        using candidateTuple = std::tuple<Standard_Integer, Standard_Real,
                                          Standard_Integer, Standard_Integer,
                                          Standard_Integer>;
        using candidateVec = std::vector<candidateTuple>;
        using surfaceTuple = std::tuple<std::string, Standard_Real, Standard_Integer,
                                        Standard_Integer>;
        using surfaceVec = std::vector<surfaceTuple>;
    public:
        Standard_Integer maxSolidsPerVoidCell;
        surfaceTuple process(const dimMap& xList, const dimMap& yList, const dimMap& zList,
                             const std::shared_ptr<VoidCell>& voidCell);
        std::tuple<Standard_Real, Standard_Real> calcCentersParameters(const dimMap& list);
        candidateTuple selectAxisSplitSurface(const dimMap& list, const centerTuple& aabbList);
        candidateTuple checkSplitSurfacePriority(candidateVec& candidates,
                                                 const dimMap& list);
    };
}

#endif //SPLITSURFACESELECTOR_HPP
