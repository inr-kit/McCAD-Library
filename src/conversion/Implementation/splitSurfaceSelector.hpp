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
        SplitSurfaceSelector();
        ~SplitSurfaceSelector();
    private:
        using dimTuple = std::tuple<Standard_Integer, Standard_Real, Standard_Real,
                                    Standard_Real>;
        using dimList = std::vector<dimTuple>;
        using centersDist = std::vector<std::tuple<Standard_Real, Standard_Real>>;
        using centerTuple = std::tuple<Standard_Real, Standard_Real, Standard_Real>;
    public:
        void process(const dimList& xList, const dimList& yList, const dimList& zList,
                     const std::shared_ptr<VoidCell>& voidCell);
        std::tuple<Standard_Real, Standard_Real> calcCentersParameters(const dimList& list);
        std::tuple<Standard_Real, Standard_Integer> selectAxisSplitSurface(
                const dimList& list, const centerTuple& aabbList);
    };
}

#endif //SPLITSURFACESELECTOR_HPP
