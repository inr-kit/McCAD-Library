// McCAD
#include "mcnpExpressionGenerator.hpp"
#include "senseEvaluator.hpp"

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    // Generate surface expressions
    double minX, minY, minZ, maxX, maxY, maxZ;
    solidObj->accessSImpl()->aabb.Get(minX, minY, minZ, maxX, maxY, maxZ);
    gp_Pnt solidCenter{(minX + std::abs(maxX-minX)/2.0),
                       (minY + std::abs(maxY-minY)/2.0),
                       (minZ + std::abs(maxZ-minZ)/2.0)};
    if(solidObj->accessSImpl()->planesList.size() > 0){
        for (const auto& plSurface : solidObj->accessSImpl()->planesList){
            if(plSurface->accessBSImpl()->generateParmts()) genPlSurfExpr(plSurface, solidCenter);
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
    TopLoc_Location location;
    GeomAdaptor_Surface surface{BRep_Tool::Surface(plSurface->accessSImpl()->face,
                                                   location)};
    gp_Pln plane = surface.Plane();
    if (plSurface->accessSImpl()->face.Orientation() == TopAbs_REVERSED){
        gp_Ax1 planeNormal = plane.Axis();
        plane.SetAxis(planeNormal.Reversed());
    }
    plSurface->accessSImpl()->surfSense =
            Tools::SenseEvaluator{}.senseRelativeToPlane(plane, solidCenter);
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
}
