// McCAD
#include "mcnpExpressionGenerator.hpp"

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    // Generate surface expressions
    if(solidObj->accessSImpl()->planesList.size() > 0){
        for (const auto& plSurface : solidObj->accessSImpl()->planesList){
            if(plSurface->accessBSImpl()->generateParmts()) genPlSurfExpr();
            else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    if(solidObj->accessSImpl()->cylindersList.size() > 0){
        for (const auto& cylSurface : solidObj->accessSImpl()->cylindersList){
            if(cylSurface->accessBSImpl()->generateParmts()) genCylSurfExpr();
            else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    if(solidObj->accessSImpl()->toriList.size() > 0){
        for (const auto& torSurface : solidObj->accessSImpl()->toriList){
            if(torSurface->accessBSImpl()->generateParmts()) genTorSurfExpr();
            else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    // Generate cell description
    genCellExpr(solidObj);
}

McCAD::Conversion::MCNPExprGenerator::~MCNPExprGenerator(){
}

void
McCAD::Conversion::MCNPExprGenerator::genPlSurfExpr(){}

void
McCAD::Conversion::MCNPExprGenerator::genCylSurfExpr(){}

void
McCAD::Conversion::MCNPExprGenerator::genTorSurfExpr(){}

void
McCAD::Conversion::MCNPExprGenerator::genCellExpr(
        const std::shared_ptr<Geometry::Solid>& solidObj){}
