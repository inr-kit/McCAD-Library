#ifndef VOIDCELLMANAGER_HPP
#define VOIDCELLMANAGER_HPP

// C++
#include <vector>
// McCAD
#include "solid_impl.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Conversion{
    class VoidCellManager {
    private:
        using shape_Name_ID = std::vector<std::tuple<TopoDS_Shape,
                                                     TCollection_ExtendedString,
                                                     Standard_Integer>>;
    public:
        VoidCellManager(const shape_Name_ID& solidsList);
        ~VoidCellManager();
    };
}
#endif //VOIDCELLMANAGER_HPP
