#ifndef SOLIDSSORTER_HPP
#define SOLIDSSORTER_HPP

// C++
#include <tuple>
#include <vector>
// OCC
#include <Standard.hxx>

namespace McCAD::Conversion{
    class SolidsSorter{
    public:
        SolidsSorter();
        ~SolidsSorter();

    private:
        using dimTuple = std::tuple<Standard_Integer, Standard_Real, Standard_Real,
                                    Standard_Real>;
        using dimList = std::vector<dimTuple>;

    public:
        void sortByElement(dimList& aList, const Standard_Integer& elementIndex);
        static Standard_Boolean byMin(const dimTuple& first, const dimTuple& second);
        static Standard_Boolean byCenter(const dimTuple& first, const dimTuple& second);
        static Standard_Boolean byMax(const dimTuple& first, const dimTuple& second);
    };
}

#endif //SOLIDSSORTER_HPP
