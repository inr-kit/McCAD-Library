#ifndef MCNPEXPRESSIONGENERATOR_HPP
#define MCNPEXPRESSIONGENERATOR_HPP

// C++
#include <memory>
// McCAD
#include "solid_impl.hpp"
// OCC

namespace McCAD::Conversion{
    class MCNPExprGenerator {
    public:
        MCNPExprGenerator();
        MCNPExprGenerator(const std::shared_ptr<Geometry::Solid>& solidObj);
        ~MCNPExprGenerator();

        void  updateSurfParmts(const std::shared_ptr<Geometry::BoundSurface>& plSurface,
                               const gp_Pnt& solidCenter);
        void genPlSurfExpr(const std::shared_ptr<Geometry::BoundSurface>& plSurface);
        void genCylSurfExpr();
        void genTorSurfExpr();
        void createSurfacesMap(const std::shared_ptr<Geometry::Solid>& solidObj);
        void genCellExpr(const std::shared_ptr<Geometry::Solid>& solidObj,
                         const Standard_Integer& cellNumber,
                         const Standard_Integer& startSurfNumber);
    };
}

#endif // MCNPEXPRESSIONGENERATOR_HPP
