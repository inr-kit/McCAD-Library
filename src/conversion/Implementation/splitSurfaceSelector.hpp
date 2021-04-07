#ifndef SPLITSURFACESELECTOR_HPP
#define SPLITSURFACESELECTOR_HPP

// C++
#include <tuple>
#include <vector>
#include <memory>
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
        using dimTuple = std::tuple<Standard_Integer, Standard_Real, Standard_Real,
                                    Standard_Real>;
        using dimList = std::vector<dimTuple>;
        using centersDist = std::vector<std::tuple<Standard_Real, Standard_Real>>;
        using centerTuple = std::tuple<Standard_Real, Standard_Real, Standard_Real>;
        using candidateTuple = std::tuple<Standard_Integer, Standard_Real,
                                          Standard_Integer, Standard_Integer,
                                          Standard_Integer>;
        using candidateVec = std::vector<candidateTuple>;
    public:
        Standard_Integer maxSolidsPerVoidCell;
        void process(const dimList& xList, const dimList& yList, const dimList& zList,
                     const std::shared_ptr<VoidCell>& voidCell);
        std::tuple<Standard_Real, Standard_Real> calcCentersParameters(const dimList& list);
        candidateTuple selectAxisSplitSurface(const dimList& list, const centerTuple& aabbList);
        candidateTuple checkSplitSurfacePriority(candidateVec& candidates,
                                                 const dimList& list);
    };
}

#endif //SPLITSURFACESELECTOR_HPP
