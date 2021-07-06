#ifndef MCNPEXPRESSIONGENERATOR_HPP
#define MCNPEXPRESSIONGENERATOR_HPP

// C++
#include <memory>
// McCAD
#include "solid_impl.hpp"
#include "voidCell.hpp"
// OCC

namespace McCAD::Conversion{
    class MCNPExprGenerator {
    public:
        MCNPExprGenerator();
        MCNPExprGenerator(const std::shared_ptr<Geometry::Solid>& solidObj,
                          const Standard_Real& precision);
        ~MCNPExprGenerator();

        void  updateSurfParmts(const std::shared_ptr<Geometry::BoundSurface>& plSurface,
                               const gp_Pnt& solidCenter);
        void genPlSurfExpr(const std::shared_ptr<Geometry::BoundSurface>& plSurface);
        void genCylSurfExpr();
        void genTorSurfExpr();
        void createSurfacesList(const std::shared_ptr<Geometry::Solid>& solidObj);
        void genCellExpr(const std::shared_ptr<Geometry::Solid>& solidObj);
        void genVoidExpr(const std::shared_ptr<VoidCell>& voidCell);
    };
}

#endif // MCNPEXPRESSIONGENERATOR_HPP
