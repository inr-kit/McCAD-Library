#ifndef VOIDCELLMANAGER_HPP
#define VOIDCELLMANAGER_HPP

// C++
#include <vector>
// McCAD
#include "solid_impl.hpp"
#include "voidCell.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Conversion{
    class VoidCellManager {
    public:
        VoidCellManager();
        ~VoidCellManager();
    private:
        std::shared_ptr<VoidCell> voidCell;
        using shape_Name_ID = std::vector<std::tuple<TopoDS_Shape,
                                                     TCollection_ExtendedString,
                                                     Standard_Integer>>;
        using dimTuple = std::tuple<Standard_Integer, Standard_Real, Standard_Real,
                                    Standard_Real>;
        using dimList = std::vector<dimTuple>;
        using surfaceTuple = std::tuple<std::string, Standard_Real, Standard_Integer,
                                        Standard_Integer>;
    public:
        dimList xAxis, yAxis, zAxis;
        void operator()(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
                        const Standard_Integer& maxSolidsPerVoidCell);
        void operator()(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
                        const Standard_Integer& maxSolidsPerVoidCell,
                        const Standard_Integer& depth, const Standard_Integer& width);
        void perform(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
                     const Standard_Integer& maxSolidsPerVoidCell);
        void populateLists(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
        void updateVoidCell(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
        void splitVoidCell(const surfaceTuple& surface);

    };
}
#endif //VOIDCELLMANAGER_HPP
