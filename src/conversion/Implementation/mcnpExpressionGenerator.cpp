// C++
#include <boost/format.hpp>
// McCAD
#include "mcnpExpressionGenerator.hpp"
#include "senseEvaluator.hpp"
// OCC
#include <Standard.hxx>

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(){}

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    // Generate surface expressions
    solidObj->accessSImpl()->calcAABBCenter();
    if(solidObj->accessSImpl()->planesList.size() > 0){
        for (const auto& plSurface : solidObj->accessSImpl()->planesList){
            if(plSurface->accessBSImpl()->generateParmts()){
                updateSurfParmts(plSurface, solidObj->accessSImpl()->aabbCenter);
                genPlSurfExpr(plSurface);
            } else throw(std::runtime_error("Error in generating surface expression!"));
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
    createSurfacesMap(solidObj);
}

McCAD::Conversion::MCNPExprGenerator::~MCNPExprGenerator(){
}

void
McCAD::Conversion::MCNPExprGenerator::updateSurfParmts(
        const std::shared_ptr<Geometry::BoundSurface>& plSurface,
        const gp_Pnt& solidCenter){
    Standard_Real x, y, z;
    plSurface->accessSImpl()->normal.Coord(x, y, z);
    /* Debug
    std::cout << "\nSolid center: x " <<  solidCenter.X() <<
                 ", y " << solidCenter.Y() <<
                 ", z " << solidCenter.Z() << std::endl;
    // Reverse components if negative.
    std::cout << "old: a " << plSurface->accessSImpl()->surfParameters[0] <<
                 ", b " << plSurface->accessSImpl()->surfParameters[1] <<
                 ", c " << plSurface->accessSImpl()->surfParameters[2] <<
                 ", d " << plSurface->accessSImpl()->surfParameters[3] << std::endl;
    std::cout << "old normal: x " << x << ", y " << y << ", z " << z << std::endl;
    */
    if (y < 0 && x == 0 && z == 0) {
        // Plane perpendicular to Y axis.
        plSurface->accessSImpl()->updated = Standard_True;
    } else if (z < 0 && x == 0 && y == 0) {
        // Plane perpendicular to Z axis.
        plSurface->accessSImpl()->updated = Standard_True;
    } else if (x < 0) {
        if (y == 0 && z == 0){
            // Plane perpendicular to X axis.
            plSurface->accessSImpl()->updated = Standard_True;
        } else {
            // General plane.
            plSurface->accessSImpl()->updated = Standard_True;
        }
    }
    if(plSurface->accessSImpl()->updated){
        // Reverse the normal direction according to MCNP convention to generate sense.
        plSurface->accessSImpl()->normal.Reverse();
        gp_Ax1 newNormal = gp_Ax1(plSurface->accessSImpl()->location, plSurface->accessSImpl()->normal);
        plSurface->accessSImpl()->plane.SetAxis(newNormal);
        plSurface->accessSImpl()->plane.Coefficients(
                    plSurface->accessSImpl()->surfParameters[0],
                    plSurface->accessSImpl()->surfParameters[1],
                    plSurface->accessSImpl()->surfParameters[2],
                    plSurface->accessSImpl()->surfParameters[3]);
        /* Debug
        std::cout << "new normal: x " << plSurface->accessSImpl()->normal.X() <<
                     ", y " << plSurface->accessSImpl()->normal.Y() <<
                     ", z " << plSurface->accessSImpl()->normal.Z() << std::endl;
        std::cout << "new: a " << plSurface->accessSImpl()->surfParameters[0] <<
                     ",b " << plSurface->accessSImpl()->surfParameters[1] <<
                     ",c " << plSurface->accessSImpl()->surfParameters[2] <<
                     ",d " << plSurface->accessSImpl()->surfParameters[3] << std::endl;
        */
    }
    plSurface->accessSImpl()->surfSense = std::signbit(Tools::SenseEvaluator{}.senseRelativeToPlane(
                     plSurface->accessSImpl()->plane, solidCenter)) ? -1 : 1;
}

void
McCAD::Conversion::MCNPExprGenerator::genPlSurfExpr(
        const std::shared_ptr<Geometry::BoundSurface>& plSurface){
    char surfExpr[255];
    // Generate planar surface expression and store in surface object.
    Standard_Real parmtA{plSurface->accessSImpl()->surfParameters[0]},
                  parmtB{plSurface->accessSImpl()->surfParameters[1]},
                  parmtC{plSurface->accessSImpl()->surfParameters[2]},
                  parmtD{plSurface->accessSImpl()->surfParameters[3]};
    if(parmtD != 0.0) parmtD *= -1;
    if ((parmtA != 0.0) && (parmtB == 0.0) && (parmtC == 0.0)){
        plSurface->accessSImpl()->surfSymb = "PX";
        std::sprintf(surfExpr, "PX%2s%15.5f\n", "", parmtD/parmtA);
    } else if ((parmtA == 0.0) && (parmtB != 0.0) && (parmtC == 0.0)){
        plSurface->accessSImpl()->surfSymb = "PY";
        std::sprintf(surfExpr, "PY%2s%15.5f\n", "", parmtD/parmtB);
    } else if ((parmtA == 0.0) && (parmtB == 0.0) && (parmtC != 0.0)){
        plSurface->accessSImpl()->surfSymb = "PZ";
        std::sprintf(surfExpr, "PZ%2s%15.5f\n", "", parmtD/parmtC);
    } else
        std::sprintf(surfExpr, "P %2s%15.5f%2s%15.5f%2s%15.5f%2s%15.5f\n", "",
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
McCAD::Conversion::MCNPExprGenerator::createSurfacesMap(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    for (const auto& surface : solidObj->accessSImpl()->facesList){
        Standard_Integer surfNumber = surface->accessSImpl()->surfaceNumber;
        if (solidObj->accessSImpl()->intersectionList.count(surfNumber) == 0)
            solidObj->accessSImpl()->intersectionList[surfNumber] = surface;
        else throw std::runtime_error("Error processing solid. Terminating!.");
        /*
        if (surface->hasAssistSurface){
            for(auto& assistSurf : surface->assistSurfaceslist){
                auto assistSurfNumber = assistSurf->accessSImpl->surfaceNumber;
                if (assistSurf->splitSurface){
                    if(!solidObj->accessSImpl()->unionList.containes(assistSurfNumber))
                        solidObj->accessSImpl()->unionList[assistSurfNumbe] = assistSurf;
                } else{
                    if(!solidObj->accessSImpl()->intersectionList.containes(assistSurfNumber))
                        solidObj->accessSImpl()->intersectionList[assistSurfNumbe] = assistSurf;
                }
            }
        }*/
    }
}

void
McCAD::Conversion::MCNPExprGenerator::genCellExpr(
        const std::shared_ptr<Geometry::Solid>& solidObj,
        const Standard_Integer& cellNumber,
        const Standard_Integer& startSurfNumber){
    std::string cellExpr{boost::str(boost::format("%d") % cellNumber)};
    if (cellExpr.size() < 5) cellExpr.resize(5, *const_cast<char*>(" "));
    // Add material
    if(solidObj->accessSImpl()->matID == 0)
        cellExpr += boost::str(boost::format(" %d")
                               % solidObj->accessSImpl()->matID);
    else
        cellExpr += boost::str(boost::format(" %1$d %2$10.5f")
                               % solidObj->accessSImpl()->matID
                               % solidObj->accessSImpl()->matDensity);
    // Loop over surfaces in intersection list and add them to epression.
    for(const auto& member : solidObj->accessSImpl()->intersectionList){
        // add surface number and expression to surfacesList.
        Standard_Integer surfaceID = member.first + startSurfNumber;
        auto surfaceIDSigned = member.second->accessSImpl()->surfSense * surfaceID;
        solidObj->accessSImpl()->surfacesList[surfaceID] = member.second;
        //the length of cell expression can not be larger than 80.
        if (cellExpr.size() + 1 + std::to_string(surfaceIDSigned).size() >= 80){
            cellExpr += "&";
        }
        cellExpr += boost::str(boost::format(" %d") % surfaceIDSigned);
    }
    cellExpr += " ";
    if(solidObj->accessSImpl()->unionList.size() > 1){
        cellExpr += "(";
        for(const auto& member : solidObj->accessSImpl()->unionList){
            // add surface number and expression to surfacesList.
            Standard_Integer surfaceID = member.first + startSurfNumber;
            auto surfaceIDSigned = member.second->accessSImpl()->surfSense * surfaceID;
            solidObj->accessSImpl()->surfacesList[surfaceID] = member.second;
            //the length of cell expression can not be larger than 80.
            if (cellExpr.size() + 1 + std::to_string(surfaceIDSigned).size() >= 80) {
                cellExpr += "&";
            } else cellExpr += ":";
            cellExpr += boost::str(boost::format(" %d") % surfaceIDSigned);
        }
        cellExpr.resize(cellExpr.size() - 1);     // Remove the last character
        cellExpr += ")";
    }
    solidObj->accessSImpl()->cellExpr = cellExpr;
}
