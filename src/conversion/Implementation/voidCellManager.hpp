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
        VoidCellManager(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
        ~VoidCellManager();
    private:
        using shape_Name_ID = std::vector<std::tuple<TopoDS_Shape,
                                                     TCollection_ExtendedString,
                                                     Standard_Integer>>;
        using dimTuple = std::tuple<Standard_Integer, Standard_Real, Standard_Real,
                                    Standard_Real>;
        using dimList = std::vector<dimTuple>;
    public:
        dimList xAxis, yAxis, zAxis;
        //VoidCell rootCellTree;
        void populateLists(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
        void createVoidCells(const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList);
    };
}
#endif //VOIDCELLMANAGER_HPP
