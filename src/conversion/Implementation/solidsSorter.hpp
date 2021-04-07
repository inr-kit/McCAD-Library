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
        using elementTuple = std::tuple<Standard_Integer, Standard_Real,
                                          Standard_Integer, Standard_Integer,
                                          Standard_Integer>;
        using elementVec = std::vector<elementTuple>;
    public:
        template<typename listType>
        listType sortByElement(listType& aList, const Standard_Integer& elementIndex);
        static Standard_Boolean byLeft(const elementTuple& first,
                                       const elementTuple& second);
        static Standard_Boolean byRight(const elementTuple& first,
                                        const elementTuple& second);
        static Standard_Boolean byIntersection(const elementTuple& first,
                                               const elementTuple& second);
    };
}

#endif //SOLIDSSORTER_HPP
