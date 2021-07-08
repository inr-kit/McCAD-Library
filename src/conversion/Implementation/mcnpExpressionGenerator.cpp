// C++
#include <boost/format.hpp>
// McCAD
#include "mcnpExpressionGenerator.hpp"
#include "senseEvaluator.hpp"
// OCC
#include <Standard.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(){}

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(
        const std::shared_ptr<Geometry::Solid>& solidObj, const Standard_Real& precision){
    // Generate surface expressions
    solidObj->accessSImpl()->calcAABBCenter();
    if(solidObj->accessSImpl()->planesList.size() > 0){
        for (const auto& plSurface : solidObj->accessSImpl()->planesList){
            if(plSurface->accessBSImpl()->generateParmts(precision)){
                updateSurfParmts(plSurface, solidObj->accessSImpl()->aabbCenter, precision);
                genPlSurfExpr(plSurface);
            } else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    if(solidObj->accessSImpl()->cylindersList.size() > 0){
        for (const auto& cylSurface : solidObj->accessSImpl()->cylindersList){
            if(cylSurface->accessBSImpl()->generateParmts(precision))
                genCylSurfExpr(cylSurface, precision);
            else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    if(solidObj->accessSImpl()->toriList.size() > 0){
        for (const auto& torSurface : solidObj->accessSImpl()->toriList){
            if(torSurface->accessBSImpl()->generateParmts(precision)) genTorSurfExpr();
            else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    // Generate cell description
    createSurfacesList(solidObj);
}

McCAD::Conversion::MCNPExprGenerator::~MCNPExprGenerator(){
}

void
McCAD::Conversion::MCNPExprGenerator::updateSurfParmts(
        const std::shared_ptr<Geometry::BoundSurface>& plSurface,
        const gp_Pnt& solidCenter, const Standard_Real& precision){
    // This function aims to adjust the normals to the planes so that they are
    // pointing in the direction of increase of all axes, MCNP style.
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
    if (std::signbit(y) && std::abs(x) < precision && std::abs(z) < precision) {
        // Plane perpendicular to Y axis.
        plSurface->accessSImpl()->updated = Standard_True;
    } else if (std::signbit(z) && std::abs(x) < precision && std::abs(y) < precision) {
        // Plane perpendicular to Z axis.
        plSurface->accessSImpl()->updated = Standard_True;
    } else if (std::signbit(x)) {
        if (std::abs(y) < precision && std::abs(z) < precision){
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
    std::string surfExpr;
    // Generate planar surface expression and store in surface object.
    Standard_Real parmtA{plSurface->accessSImpl()->surfParameters[0]},
                  parmtB{plSurface->accessSImpl()->surfParameters[1]},
                  parmtC{plSurface->accessSImpl()->surfParameters[2]},
                  parmtD{plSurface->accessSImpl()->surfParameters[3]};
    if(parmtD != 0.0) parmtD *= -1;
    if ((parmtA != 0.0) && (parmtB == 0.0) && (parmtC == 0.0)){
        plSurface->accessSImpl()->surfSymb = "PX";
        surfExpr += boost::str(boost::format("PX %11.5f") % ((parmtD/parmtA)/10.0));
    } else if ((parmtA == 0.0) && (parmtB != 0.0) && (parmtC == 0.0)){
        plSurface->accessSImpl()->surfSymb = "PY";
        surfExpr += boost::str(boost::format("PY %11.5f") % ((parmtD/parmtB)/10.0));
    } else if ((parmtA == 0.0) && (parmtB == 0.0) && (parmtC != 0.0)){
        plSurface->accessSImpl()->surfSymb = "PZ";
        surfExpr += boost::str(boost::format("PZ %11.5f") % ((parmtD/parmtC)/10.0));
    } else
        surfExpr += boost::str(boost::format("P %11.5f  %11.5f  %11.5f  %11.5f")
                           % (parmtA/10.0) % (parmtB/10.0) % (parmtC/10.0)
                           % (parmtD/10.0));
    plSurface->accessSImpl()->surfExpr = surfExpr;
}

void
McCAD::Conversion::MCNPExprGenerator::genCylSurfExpr(
        const std::shared_ptr<Geometry::BoundSurface>& cylSurface,
        const Standard_Real& precision){
    std::string surfExpr;
    // Generate planar surface expression and store in surface object.
    // Get the diretion of the symmetry axis of the cylinder.
    gp_Dir cylAxisDir = cylSurface->accessSImpl()->symmetryAxis;
    gp_Pnt cylLocation = cylSurface->accessSImpl()->location;
    // Check if parallet to X-axis
    if (std::abs(cylAxisDir.Y()) < precision && std::abs(cylAxisDir.Z()) < precision) {
        // Cylinder parallel to X-axis.
        // Check location.
        if(std::abs(cylLocation.Y()) < precision && std::abs(cylLocation.Z()) < precision){
            // Cylinder on X-axis.
            cylSurface->accessSImpl()->surfSymb = "CX";
            surfExpr += boost::str(boost::format("CX %11.5f") % (cylSurface->accessSImpl()->radius));
        } else {
            // Cylinder parallel to X-axis.
            cylSurface->accessSImpl()->surfSymb = "C/X";
            surfExpr += boost::str(boost::format("C/X %11.5f  %11.5f  %11.5f")
                                   % cylLocation.Y()
                                   % cylLocation.Z()
                                   % (cylSurface->accessSImpl()->radius));
        }
    } else if (std::abs(cylAxisDir.X()) < precision && std::abs(cylAxisDir.Z()) < precision) {
        // Cylinder parallel to Y-axis.
        // Check location.
        if(std::abs(cylLocation.X()) < precision && std::abs(cylLocation.Z()) < precision){
            // Cylinder on Y-axis.
            cylSurface->accessSImpl()->surfSymb = "CY";
            surfExpr += boost::str(boost::format("CY %11.5f") % (cylSurface->accessSImpl()->radius));
        } else {
            // Cylinder parallel to Y-axis.
            cylSurface->accessSImpl()->surfSymb = "C/Y";
            surfExpr += boost::str(boost::format("C/Y %11.5f  %11.5f  %11.5f")
                                   % cylLocation.X()
                                   % cylLocation.Z()
                                   % (cylSurface->accessSImpl()->radius));
        }
    } else if (std::abs(cylAxisDir.X()) < precision && std::abs(cylAxisDir.Y()) < precision) {
        // Cylinder parallel to Z-axis.
        // Check location.
        if(std::abs(cylLocation.X()) < precision && std::abs(cylLocation.Y()) < precision){
            // Cylinder on Z-axis.
            cylSurface->accessSImpl()->surfSymb = "CZ";
            surfExpr += boost::str(boost::format("CZ %11.5f") % (cylSurface->accessSImpl()->radius));
        } else {
            // Cylinder parallel to Z-axis.
            cylSurface->accessSImpl()->surfSymb = "C/Z";
            surfExpr += boost::str(boost::format("C/Z %11.5f  %11.5f  %11.5f")
                                   % cylLocation.X()
                                   % cylLocation.Y()
                                   % (cylSurface->accessSImpl()->radius));
        }
    } else {
        // General cylinder.
        cylSurface->accessSImpl()->surfSymb = "GQ";
        surfExpr += boost::str(boost::format("GQ %11.5f  %11.5f  %11.5f %11.5f  %11.5f  "
                                             "%11.5f %11.5f  %11.5f  %11.5f  %11.5f")
                               % cylSurface->accessSImpl()->surfParameters[0]
                % cylSurface->accessSImpl()->surfParameters[1]
                % cylSurface->accessSImpl()->surfParameters[2]
                % cylSurface->accessSImpl()->surfParameters[3]
                % cylSurface->accessSImpl()->surfParameters[4]
                % cylSurface->accessSImpl()->surfParameters[5]
                % cylSurface->accessSImpl()->surfParameters[6]
                % cylSurface->accessSImpl()->surfParameters[7]
                % cylSurface->accessSImpl()->surfParameters[8]
                % cylSurface->accessSImpl()->surfParameters[9]);
    }
    cylSurface->accessSImpl()->surfExpr = surfExpr;
}

void
McCAD::Conversion::MCNPExprGenerator::genTorSurfExpr(){}

void
McCAD::Conversion::MCNPExprGenerator::createSurfacesList(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    for (const auto& surface : solidObj->accessSImpl()->facesList){
        solidObj->accessSImpl()->intersectionList.push_back(surface);
        if (surface->accessSImpl()->hasAssistSurface){
            /*for(const auto& assistSurf : surface->accessSImpl()->assistSurfaceslist){
                if (assistSurf->accessSImpl()->splitSurface){
                    solidObj->accessSImpl()->unionList.push_back(assistSurf);
                } else{
                    solidObj->accessSImpl()->intersectionList.push_back(assistSurf);;
                }
            }*/
        }
    }
}

void
McCAD::Conversion::MCNPExprGenerator::genCellExpr(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    std::string cellExpr, complimentExpr;
    complimentExpr += " (";
    for(const auto& surface : solidObj->accessSImpl()->intersectionList){
        // add surface number and expression to surfacesList.
        auto surfaceIDSigned = surface->accessSImpl()->surfSense *
                surface->accessSImpl()->uniqueID;
        cellExpr += boost::str(boost::format("%d ") % surfaceIDSigned);
        complimentExpr += boost::str(boost::format("%d : ") % (-1*surfaceIDSigned));
    }
    cellExpr.resize(cellExpr.size() - 1);
    complimentExpr.resize(complimentExpr.size() - 3);
    complimentExpr += ")";
    if(solidObj->accessSImpl()->unionList.size() > 1){
        cellExpr += " (";
        for(const auto& surface : solidObj->accessSImpl()->unionList){
            // add surface number and expression to surfacesList.
            auto surfaceIDSigned = surface->accessSImpl()->surfSense *
                    surface->accessSImpl()->uniqueID;
            cellExpr += boost::str(boost::format("%d : ") % surfaceIDSigned);
            complimentExpr += boost::str(boost::format("%d ") % (-1*surfaceIDSigned));
        }
        cellExpr.resize(cellExpr.size() - 3);     // Remove the last character
        cellExpr += ")";
        complimentExpr.resize(complimentExpr.size() - 1);
    }
    solidObj->accessSImpl()->cellExpr = cellExpr;
    solidObj->accessSImpl()->complimentExpr = complimentExpr;
}

void
McCAD::Conversion::MCNPExprGenerator::genVoidExpr(const std::shared_ptr<VoidCell>& voidCell){
    std::string voidSurfExpr;
    voidSurfExpr = boost::str(boost::format("RPP %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f")
                          % (voidCell->minX / 10.0) % (voidCell->maxX / 10.0)
                          % (voidCell->minY / 10.0) % (voidCell->maxY / 10.0)
                          % (voidCell->minZ / 10.0) % (voidCell->maxZ / 10.0));
    voidCell->voidSurfExpr = voidSurfExpr;
}
