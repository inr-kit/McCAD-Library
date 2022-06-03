#ifndef SOLIDSSORTER_HPP
#define SOLIDSSORTER_HPP

// C++
#include <tuple>
#include <vector>
#include <string>

namespace McCAD::Conversion{
    class SolidsSorter{
    public:
        SolidsSorter();
        ~SolidsSorter();
    private:
        //  elementTuple format: <solids to the left, position along axis,
        //                        solids to the right, intersections,
        //                        expected numbers of splittings>
        using elementTuple = std::tuple<int, double, int, int, int>;
        using elementVec = std::vector<elementTuple>;
        // elementTuple2 format: <cutting plane axis tag, position along the axis
        //                        number of intersections, number of expected splittings>
        using elementTuple2 = std::tuple<std::string, double, int, int>;
    public:
        template<typename listType>
        listType sortByElement(listType& aList, const int& elementIndex);
        template<typename listType>
        listType sortByElement2(listType& aList, const int& elementIndex);

        static bool byLeft(const elementTuple& first, const elementTuple& second);
        static bool byRight(const elementTuple& first, const elementTuple& second);
        static bool byIntersection(const elementTuple& first, const elementTuple& second);
        static bool byIntersection2(const elementTuple2& first, const elementTuple2& second);
        static bool bySplitting2(const elementTuple2& first, const elementTuple2& second);
    };
}
#include "solidsSorter.tpp"

#endif //SOLIDSSORTER_HPP
