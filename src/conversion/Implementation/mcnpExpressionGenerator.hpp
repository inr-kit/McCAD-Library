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
        MCNPExprGenerator(const std::shared_ptr<Geometry::Solid>& solidObj);
        ~MCNPExprGenerator();

        void genPlSurfExpr();
        void genCylSurfExpr();
        void genTorSurfExpr();
        void genCellExpr(const std::shared_ptr<Geometry::Solid>& solidObj);
    };
}

#endif // MCNPEXPRESSIONGENERATOR_HPP
