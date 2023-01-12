#ifndef SOLIDREPAIRER_HPP
#define SOLIDREPAIRER_HPP

// OCCT
#include <TopoDS_Solid.hxx>

namespace McCAD::Decomposition{

    class SolidRepairer{

    public:
        bool operator()(TopoDS_Solid& solid) const;

        template<typename Container>
        bool operator()(Container& solids) const;

    };

}

#include "SolidRepairer.tpp"

#endif
