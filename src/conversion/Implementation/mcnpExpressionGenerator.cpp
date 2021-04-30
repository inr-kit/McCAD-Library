// McCAD
#include "mcnpExpressionGenerator.hpp"
#include "senseEvaluator.hpp"
// OCC
#include <Standard.hxx>

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    // Generate surface expressions
    solidObj->accessSImpl()->calcAABBCenter();
    if(solidObj->accessSImpl()->planesList.size() > 0){
        for (const auto& plSurface : solidObj->accessSImpl()->planesList){
            if(plSurface->accessBSImpl()->generateParmts()) genPlSurfExpr(
                        plSurface, solidObj->accessSImpl()->aabbCenter);
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
McCAD::Conversion::MCNPExprGenerator::genPlSurfExpr(
        const std::shared_ptr<Geometry::BoundSurface>& plSurface,
        const gp_Pnt& solidCenter){
    char surfExpr[255];
    // Generate planar surface expression and store in surface object.
    plSurface->accessSImpl()->surfSense = std::signbit(
            Tools::SenseEvaluator{}(plSurface->accessSImpl()->face, solidCenter)) ?
                -1 : 1;
    Standard_Real parmtA{plSurface->accessSImpl()->surfParameters[0]},
                  parmtB{plSurface->accessSImpl()->surfParameters[1]},
                  parmtC{plSurface->accessSImpl()->surfParameters[2]},
                  parmtD{plSurface->accessSImpl()->surfParameters[3]};
    if(parmtD != 0.0) parmtD *= -1;
    if ((parmtA != 0.0) && (parmtB == 0.0) && (parmtC == 0.0)){
        plSurface->accessSImpl()->surfSymb = "PX";
        std::sprintf(surfExpr, "PX%8s%15.7f\n", "", parmtD/parmtA);
    } else if ((parmtA == 0.0) && (parmtB != 0.0) && (parmtC == 0.0)){
        plSurface->accessSImpl()->surfSymb = "PY";
        std::sprintf(surfExpr, "PY%8s%15.7f\n", "", parmtD/parmtB);
    } else if ((parmtA == 0.0) && (parmtB == 0.0) && (parmtC != 0.0)){
        plSurface->accessSImpl()->surfSymb = "PZ";
        std::sprintf(surfExpr, "PZ%8s%15.7f\n", "", parmtD/parmtC);
    } else
        std::sprintf(surfExpr, "P %8s%15.7f%5s%15.7f%5s%15.7f%5s%15.7f\n", "",
                     parmtA, "", parmtB, "", parmtC, "", parmtD);
    plSurface->accessSImpl()->surfExpr = surfExpr;
    plSurface->accessSImpl()->surfExpr.LeftAdjust();
    plSurface->accessSImpl()->surfExpr.RightAdjust();
}

void
McCAD::Conversion::MCNPExprGenerator::genCylSurfExpr(){}

void
McCAD::Conversion::MCNPExprGenerator::genTorSurfExpr(){}

void
McCAD::Conversion::MCNPExprGenerator::genCellExpr(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    // Generate Cell Expression
    auto aabbCenter = solidObj->accessSImpl()->aabbCenter;
    auto boxDiagLength = solidObj->accessSImpl()->boxDiagonalLength;
    gp_Pnt xPoint{aabbCenter}, yPoint{aabbCenter}, zPoint{aabbCenter};
    xPoint.SetX(aabbCenter.X() + 1000.0*boxDiagLength);
    yPoint.SetY(aabbCenter.Y() + 100.0*boxDiagLength);
    zPoint.SetZ(aabbCenter.Z() + 100.0*boxDiagLength);
    for (const auto& surface : solidObj->accessSImpl()->facesList){
        Standard_Boolean xReversed{Standard_False}, yReversed{Standard_False},
                         zReversed{Standard_False};
        xReversed = std::signbit(Tools::SenseEvaluator{}(
                                    surface->accessSImpl()->face, xPoint)) ?
                    Standard_False : Standard_True;
        yReversed = std::signbit(Tools::SenseEvaluator{}(
                                    surface->accessSImpl()->face, yPoint)) ?
                    Standard_False : Standard_True;
        zReversed = std::signbit(Tools::SenseEvaluator{}(
                                    surface->accessSImpl()->face, zPoint)) ?
                    Standard_False : Standard_True;
        surface->accessSImpl()->surfSense = (xReversed || yReversed || zReversed) ? -1 : 1;
    }
}
