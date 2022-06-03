#ifndef MCNPEXPRESSIONGENERATOR_HPP
#define MCNPEXPRESSIONGENERATOR_HPP

// C++
#include <memory>
// McCAD
#include "solid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "voidCell.hpp"
// OCCT
#include <gp_Pnt.hxx>

namespace McCAD::Conversion{
    class MCNPExprGenerator {
    public:
        MCNPExprGenerator();
        MCNPExprGenerator(const double& precision, const double& scalingFactor);
        ~MCNPExprGenerator();

    private:
        double precision{1.0e-6}, scalingFactor{1.0};
    public:
        void operator()(const std::shared_ptr<Geometry::Solid>& solidObj);
        void operator()(const std::shared_ptr<VoidCell>& voidCell);
        void  updateSurfParmts(const std::shared_ptr<Geometry::BoundSurface>& plSurface,
                               const double& precision);
        void genPlSurfExpr(const std::shared_ptr<Geometry::BoundSurface>& plSurface,
                           const gp_Pnt& solidCenter, const double& precision);
        void genCylSurfExpr(const std::shared_ptr<Geometry::BoundSurface>& cylSurface,
                            const double& precision);
        void genTorSurfExpr(const std::shared_ptr<Geometry::BoundSurface>& torSurface,
                            const double& precision);
        void createSurfacesList(const std::shared_ptr<Geometry::Solid>& solidObj);
        void genCellExpr(const std::shared_ptr<Geometry::Solid>& solidObj);
        void genVoidExpr(const std::shared_ptr<VoidCell>& voidCell);
    };
}

#endif // MCNPEXPRESSIONGENERATOR_HPP
