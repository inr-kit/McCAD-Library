// C++
#include <boost/format.hpp>
// McCAD
#include "mcnpExpressionGenerator.hpp"
#include "senseEvaluator.hpp"
// OCCT
#include <gp_Dir.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(){}

McCAD::Conversion::MCNPExprGenerator::MCNPExprGenerator(
        const double& precision, const double& angularTolerance, const double& scalingFactor) :
    precision{ precision }, angularTolerance{ angularTolerance }, scalingFactor {scalingFactor} {
}

McCAD::Conversion::MCNPExprGenerator::~MCNPExprGenerator(){
}

/** ********************************************************************
* @brief   The operator calls the specialized writer functions of cell, surface, and data cards.
* @param   solidObj is a solid object.
* @date    31/12/2021
* @modified 9/12/2022
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::operator()(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    // Generate surface expressions
    // Calculate center of mass to get a sense of surfaces of the solid.
    GProp_GProps geoProps;
    BRepGProp::VolumeProperties(solidObj->accessSImpl()->solidShape, geoProps);
    gp_Pnt solidCenter = geoProps.CentreOfMass();
    if(solidObj->accessSImpl()->aabb.IsOut(solidCenter)){
        // Consider the center of the AABB instead.
        solidObj->accessSImpl()->calcAABBCenter();
        solidCenter = solidObj->accessSImpl()->aabbCenter;
    }
    // All generated parameters of surfaces are scaled to the output units specified
    // by the user in the config file. solidCenter needs to be scaled accordingly.
    if(scalingFactor != 1.0){
        // Scale the solid center.
        solidCenter.Scale(gp_Pnt{0.0, 0.0, 0.0}, scalingFactor);
    }
    if(solidObj->accessSImpl()->planesList.size() > 0){
        for (const auto& plSurface : solidObj->accessSImpl()->planesList){
            if(plSurface->accessBSImpl()->generateParmts(precision, scalingFactor)){
                updateSurfParmts(plSurface, precision);
                genPlSurfExpr(plSurface, solidCenter, precision);
            } else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    if(solidObj->accessSImpl()->cylindersList.size() > 0){
        for (const auto& cylSurface : solidObj->accessSImpl()->cylindersList){
            if(cylSurface->accessBSImpl()->generateParmts(precision, scalingFactor))
                genCylSurfExpr(cylSurface, precision);
            else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    if(solidObj->accessSImpl()->toriList.size() > 0){
        for (const auto& torSurface : solidObj->accessSImpl()->toriList){
            if(torSurface->accessBSImpl()->generateParmts(precision, scalingFactor))
                genTorSurfExpr(torSurface, precision);
            else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    if (solidObj->accessSImpl()->conesList.size() > 0) {
        for (const auto& coneSurface : solidObj->accessSImpl()->conesList) {
            if (coneSurface->accessBSImpl()->generateParmts(precision, scalingFactor))
                genConeSurfExpr(coneSurface, precision, angularTolerance);
            else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    // Process assisting surfaces.
    if(solidObj->accessSImpl()->assistFacesList.size() > 0){
        for (const auto& assistPlnSurface : solidObj->accessSImpl()->assistFacesList){
            if(assistPlnSurface->accessBSImpl()->generateParmts(precision, scalingFactor)){
                updateSurfParmts(assistPlnSurface, precision);
                genPlSurfExpr(assistPlnSurface, solidCenter, precision);
            } else throw(std::runtime_error("Error in generating surface expression!"));
        }
    }
    // Generate cell description.
    createSurfacesList(solidObj);
}

/** ********************************************************************
* @brief   The operator calls the specialized writer functions of void cells.
* @param   voidCell is a void cell to generate the surface expressions for.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::operator()(
        const std::shared_ptr<VoidCell>& voidCell){
    // Generate void expression
    genVoidExpr(voidCell);
}

/** ********************************************************************
* @brief   The function updates surface parameters so that normals point in the positive half space in the X direction.
* @param   plSurface is a planar surface.
* @param   precision is the precision specified on the config file.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::updateSurfParmts(
        const std::shared_ptr<Geometry::BoundSurface>& plSurface,
        const double& precision){
    // This function aims to adjust the normals to the planes so that they are
    // pointing in the direction of increase of all axes, MCNP style.
    double x, y, z;
    plSurface->accessSImpl()->normal.Coord(x, y, z);
    if (std::signbit(y) && std::abs(x) < precision && std::abs(z) < precision) {
        // Plane perpendicular to Y axis.
        plSurface->accessSImpl()->updated = true;
    } else if (std::signbit(z) && std::abs(x) < precision && std::abs(y) < precision) {
        // Plane perpendicular to Z axis.
        plSurface->accessSImpl()->updated = true;
    } else if (std::signbit(x)) {
        if (std::abs(y) < precision && std::abs(z) < precision){
            // Plane perpendicular to X axis.
            plSurface->accessSImpl()->updated = true;
        } else {
            // General plane.
            plSurface->accessSImpl()->updated = true;
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
    }
}

/** ********************************************************************
* @brief   The function generates the MCNP surface expression of a planar surface.
* @param   plSurface is a planar surface.
* @param   solidCenter is the center of mass of the solid.
* @param   precision is the precision specified on the config file.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::genPlSurfExpr(
        const std::shared_ptr<Geometry::BoundSurface>& plSurface,
        const gp_Pnt& solidCenter, const double& precision){
    // Parameters calculated in Open Cascade:
    // A * X + B * Y + C * Z + D = 0.0
    // Parameters used by MCNP.
    // AX + BY + CZ - D = 0.0
    // Calculate plane sense.
    plSurface->accessSImpl()->surfSense = std::signbit(Tools::SenseEvaluator{}.senseRelativeToPlane(
                     plSurface->accessSImpl()->plane, solidCenter)) ? -1 : 1;
    std::string surfExpr;
    // Generate planar surface expression and store in surface object.
    //Note that the sifn of parameter D is changed!
    double parmtA{plSurface->accessSImpl()->surfParameters[0]},
                  parmtB{plSurface->accessSImpl()->surfParameters[1]},
                  parmtC{plSurface->accessSImpl()->surfParameters[2]},
                  parmtD{-1.0*plSurface->accessSImpl()->surfParameters[3]};
    if ((std::abs(parmtA) >= precision) && (std::abs(parmtB) < precision) &&
            (std::abs(parmtC) < precision)){
        plSurface->accessSImpl()->surfSymb = "PX";
        surfExpr += boost::str(boost::format("PX %13.7f") % (parmtD/parmtA));
        plSurface->accessSImpl()->surfMcxPar = std::make_tuple("plane-x", std::to_string(parmtD / parmtA), "transmission");
    } else if ((std::abs(parmtA) < precision) && (std::abs(parmtB) >= precision) &&
               (std::abs(parmtC) < precision)){
        plSurface->accessSImpl()->surfSymb = "PY";
        surfExpr += boost::str(boost::format("PY %13.7f") % (parmtD/parmtB));
        plSurface->accessSImpl()->surfMcxPar = std::make_tuple("plane-y", std::to_string(parmtD / parmtB), "transmission");
    } else if ((std::abs(parmtA) < precision) && (std::abs(parmtB) < precision) &&
               (std::abs(parmtC) >= precision)){
        plSurface->accessSImpl()->surfSymb = "PZ";
        surfExpr += boost::str(boost::format("PZ %13.7f") % (parmtD/parmtC));
        plSurface->accessSImpl()->surfMcxPar = std::make_tuple("plane-z", std::to_string(parmtD / parmtC), "transmission");
    }
    else {
        surfExpr += boost::str(boost::format("P %13.7f  %13.7f  %13.7f  %13.7f")
            % parmtA % parmtB % parmtC % parmtD);
        std::string coeffs = std::to_string(parmtA) + " " +
                             std::to_string(parmtB) + " " +
                             std::to_string(parmtC) + " " +
                             std::to_string(parmtD);
        plSurface->accessSImpl()->surfMcxPar = std::make_tuple("plane", coeffs.c_str(), "transmission");
    }
    plSurface->accessSImpl()->surfExpr = surfExpr;
}

/** ********************************************************************
* @brief   The function generates the MCNP surface expression of a cylindrical surface.
* @param   cylSurface is a cylindrical surface.
* @param   precision is the precision specified on the config file.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::genCylSurfExpr(
        const std::shared_ptr<Geometry::BoundSurface>& cylSurface,
        const double& precision){
    // Parameters calculated in OpenCascade:
    // A1.X**2 + A2.Y**2 + A3.Z**2 + 2.(B1.X.Y + B2.X.Z + B3.Y.Z) + 2.(C1.X + C2.Y + C3.Z) + D = 0.0
    // Parameters used by MCNP.
    // A.X**2 + B.Y**2 + C.Z**2 + D.XY + E.YZ + F.ZX + G.X + H.Y + J.Z + K = 0.0
    std::string surfExpr;
    // Generate cylindrical surface expression and store in surface object.
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
            surfExpr += boost::str(boost::format("CX %13.7f")
                                   % cylSurface->accessSImpl()->radius);
            std::string coeffs = "0.0 0.0 "  + std::to_string(cylSurface->accessSImpl()->radius);
            cylSurface->accessSImpl()->surfMcxPar = std::make_tuple("cylinder-x", coeffs.c_str(), "transmission");
        } else {
            // Cylinder parallel to X-axis.
            cylSurface->accessSImpl()->surfSymb = "C/X";
            surfExpr += boost::str(boost::format("C/X %13.7f  %13.7f  %13.7f")
                                   % cylLocation.Y() % cylLocation.Z()
                                   % cylSurface->accessSImpl()->radius);
            std::string coeffs = std::to_string(cylLocation.Y())+" " + std::to_string(cylLocation.Z()) + " " + std::to_string(cylSurface->accessSImpl()->radius);
            cylSurface->accessSImpl()->surfMcxPar = std::make_tuple("cylinder-x", coeffs.c_str(), "transmission");
        }
    } else if (std::abs(cylAxisDir.X()) < precision && std::abs(cylAxisDir.Z()) < precision) {
        // Cylinder parallel to Y-axis.
        // Check location.
        if(std::abs(cylLocation.X()) < precision && std::abs(cylLocation.Z()) < precision){
            // Cylinder on Y-axis.
            cylSurface->accessSImpl()->surfSymb = "CY";
            surfExpr += boost::str(boost::format("CY %13.7f")
                                   % cylSurface->accessSImpl()->radius);
            std::string coeffs = "0.0 0.0 " + std::to_string(cylSurface->accessSImpl()->radius);
            cylSurface->accessSImpl()->surfMcxPar = std::make_tuple("cylinder-y", coeffs.c_str(), "transmission");
        } else {
            // Cylinder parallel to Y-axis.
            cylSurface->accessSImpl()->surfSymb = "C/Y";
            surfExpr += boost::str(boost::format("C/Y %13.7f  %13.7f  %13.7f")
                                   % cylLocation.X() % cylLocation.Z()
                                   % cylSurface->accessSImpl()->radius);
            std::string coeffs = std::to_string(cylLocation.X()) + " " + std::to_string(cylLocation.Z()) + " " + std::to_string(cylSurface->accessSImpl()->radius);
            cylSurface->accessSImpl()->surfMcxPar = std::make_tuple("cylinder-y", coeffs.c_str(), "transmission");
        }
    } else if (std::abs(cylAxisDir.X()) < precision && std::abs(cylAxisDir.Y()) < precision) {
        // Cylinder parallel to Z-axis.
        // Check location.
        if(std::abs(cylLocation.X()) < precision && std::abs(cylLocation.Y()) < precision){
            // Cylinder on Z-axis.
            cylSurface->accessSImpl()->surfSymb = "CZ";
            surfExpr += boost::str(boost::format("CZ %13.7f")
                                   % cylSurface->accessSImpl()->radius);
            std::string coeffs = "0.0 0.0 " + std::to_string(cylSurface->accessSImpl()->radius);
            cylSurface->accessSImpl()->surfMcxPar = std::make_tuple("cylinder-z", coeffs.c_str(), "transmission");
        } else {
            // Cylinder parallel to Z-axis.
            cylSurface->accessSImpl()->surfSymb = "C/Z";
            surfExpr += boost::str(boost::format("C/Z %13.7f  %13.7f  %13.7f")
                                   % cylLocation.X() % cylLocation.Y()
                                   % cylSurface->accessSImpl()->radius);
            std::string coeffs = std::to_string(cylLocation.X()) + " " + std::to_string(cylLocation.Y()) + " " + std::to_string(cylSurface->accessSImpl()->radius);
            cylSurface->accessSImpl()->surfMcxPar = std::make_tuple("cylinder-z", coeffs.c_str(), "transmission");
        }
    } else {
        // General cylinder.
        double parmtA{cylSurface->accessSImpl()->surfParameters[0]},
                      parmtB{cylSurface->accessSImpl()->surfParameters[1]},
                      parmtC{cylSurface->accessSImpl()->surfParameters[2]},
                      parmtD{2*cylSurface->accessSImpl()->surfParameters[3]},
                      parmtE{2*cylSurface->accessSImpl()->surfParameters[5]},
                      parmtF{2*cylSurface->accessSImpl()->surfParameters[4]},
                      parmtG{2*cylSurface->accessSImpl()->surfParameters[6]},
                      parmtH{2*cylSurface->accessSImpl()->surfParameters[7]},
                      parmtJ{2*cylSurface->accessSImpl()->surfParameters[8]},
                      parmtK{cylSurface->accessSImpl()->surfParameters[9]};
        cylSurface->accessSImpl()->surfSymb = "GQ";
        surfExpr += boost::str(boost::format("GQ %13.7f  %13.7f  %13.7f %13.7f  %13.7f  "
                                             "%13.7f %13.7f  %13.7f  %13.7f  %13.7f")
                               % parmtA % parmtB % parmtC % parmtD % parmtE
                               % parmtF % parmtG % parmtH % parmtJ % parmtK);
        std::string coeffs = std::to_string(parmtA) + " " + 
                             std::to_string(parmtB) + " " + 
                             std::to_string(parmtC) + " " + 
                             std::to_string(parmtD) + " " + 
                             std::to_string(parmtE) + " " +
                             std::to_string(parmtF) + " " + 
                             std::to_string(parmtG) + " " + 
                             std::to_string(parmtH) + " " + 
                             std::to_string(parmtJ) + " " + 
                             std::to_string(parmtK);
        cylSurface->accessSImpl()->surfMcxPar = std::make_tuple("gq", coeffs.c_str(), "transmission");
    }
    cylSurface->accessSImpl()->surfExpr = surfExpr;
}

/** ********************************************************************
* @brief   The function generates the MCNP surface expression of a toroidal surface.
* @param   torSurface is a toroidal surface.
* @param   precision is the precision specified on the config file.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::genTorSurfExpr(
        const std::shared_ptr<Geometry::BoundSurface>& torSurface,
        const double& precision){
    std::string surfExpr;
    // Generate toroidal surface expression and store in surface object.
    // Get the diretion of the symmetry axis of the torus.
    gp_Dir torAxisDir = torSurface->accessSImpl()->symmetryAxis;
    gp_Pnt torLocation = torSurface->accessSImpl()->location;
    // Check if parallet to X-axis
    if (std::abs(torAxisDir.Y()) < precision && std::abs(torAxisDir.Z()) < precision) {
        // Torus is parallel to X-axis.
        torSurface->accessSImpl()->surfSymb = "TX";
        surfExpr += boost::str(boost::format("TX %13.7f  %13.7f  %13.7f  %13.7f  "
                                             "%13.7f  %13.7f")
                               % torLocation.X() % torLocation.Y() % torLocation.Z()
                               % torSurface->accessSImpl()->majorRadius
                               % torSurface->accessSImpl()->minorRadius
                               % torSurface->accessSImpl()->minorRadius);
        std::string coeffs =
            std::to_string(torLocation.X()) + " " +
            std::to_string(torLocation.Y()) + " " +
            std::to_string(torLocation.Z()) + " " +
            std::to_string(torSurface->accessSImpl()->majorRadius) + " " +
            std::to_string(torSurface->accessSImpl()->minorRadius) + " " +
            std::to_string(torSurface->accessSImpl()->minorRadius)  ;
        torSurface->accessSImpl()->surfMcxPar = std::make_tuple("torus-x", coeffs.c_str(), "transmission");
    } else if (std::abs(torAxisDir.X()) < precision && std::abs(torAxisDir.Z()) < precision) {
        // Torus is parallel to Y-axis.
        torSurface->accessSImpl()->surfSymb = "TY";
        surfExpr += boost::str(boost::format("TY %13.7f  %13.7f  %13.7f  %13.7f  "
                                             "%13.7f  %13.7f")
                               % torLocation.X() % torLocation.Y() % torLocation.Z()
                               % torSurface->accessSImpl()->majorRadius
                               % torSurface->accessSImpl()->minorRadius
                               % torSurface->accessSImpl()->minorRadius);
        std::string coeffs =
            std::to_string(torLocation.X()) + " " +
            std::to_string(torLocation.Y()) + " " +
            std::to_string(torLocation.Z()) + " " +
            std::to_string(torSurface->accessSImpl()->majorRadius) + " " +
            std::to_string(torSurface->accessSImpl()->minorRadius) + " " +
            std::to_string(torSurface->accessSImpl()->minorRadius);
        torSurface->accessSImpl()->surfMcxPar = std::make_tuple("torus-y", coeffs.c_str(), "transmission");

    } else if (std::abs(torAxisDir.X()) < precision && std::abs(torAxisDir.Y()) < precision) {
        // Torus is parallel to Z-axis.
        torSurface->accessSImpl()->surfSymb = "TZ";
        surfExpr += boost::str(boost::format("TZ %13.7f  %13.7f  %13.7f  %13.7f  "
                                             "%13.7f  %13.7f")
                               % torLocation.X() % torLocation.Y() % torLocation.Z()
                               % torSurface->accessSImpl()->majorRadius
                               % torSurface->accessSImpl()->minorRadius
                               % torSurface->accessSImpl()->minorRadius);
        std::string coeffs =
            std::to_string(torLocation.X()) + " " +
            std::to_string(torLocation.Y()) + " " +
            std::to_string(torLocation.Z()) + " " +
            std::to_string(torSurface->accessSImpl()->majorRadius) + " " +
            std::to_string(torSurface->accessSImpl()->minorRadius) + " " +
            std::to_string(torSurface->accessSImpl()->minorRadius);
        torSurface->accessSImpl()->surfMcxPar = std::make_tuple("torus-z", coeffs.c_str(), "transmission");
    } else throw std::runtime_error("Tori with a symmetry axis not parallel to X/Y/Z "
                                    "is not supported in MCNP. Please turn on simplifyTori"
                                    "option on the input config and rerun decomposition."
                                    "Conversion terminated!");
    torSurface->accessSImpl()->surfExpr = surfExpr;
}

/** ********************************************************************
* @brief   The function generates the MCNP surface expression of a conical surface.
* @param   coneSurface is a conical surface.
* @param   precision is the precision specified on the config file.
* @date    9/12/2022
* @author  Jiandi Guo
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::genConeSurfExpr(
    const std::shared_ptr<Geometry::BoundSurface>& coneSurface,
    const double& precision, const double& angularTolerance) {
    std::string surfExpr;
    // Generate toroidal surface expression and store in surface object.
    // Get the diretion of the symmetry axis of the torus.
    gp_Dir coneAxisDir = coneSurface->accessSImpl()->symmetryAxis;
    Standard_Real SemiAngle = coneSurface->accessSImpl()->SemiAngle;
    gp_Pnt coneLocation = coneSurface->accessSImpl()->location;
    gp_Dir cone_Dir = coneSurface->accessSImpl()->cone_Dir;
    double X, Y, Z;
    X = coneLocation.X() / 10;
    Y = coneLocation.Y() / 10;
    Z = coneLocation.Z() / 10;


    // Check if parallet to X-axis
    if (std::abs(coneAxisDir.Y()) < precision && std::abs(coneAxisDir.Z()) < precision) {
        //for necp-mcx surface expression
        std::string coeffs;
        //cone parallel to X-axis.
        if (abs(coneLocation.X()) >= precision && abs(coneLocation.Y()) <= precision && abs(coneLocation.Z()) <= precision) {
            coneSurface->accessSImpl()->surfSymb = "KX";
            surfExpr += boost::str(boost::format("KX %13.7f  %13.7f")
                % X
                % pow(tan(SemiAngle),2.0));
            coeffs = std::to_string(X) + " 0.0 0.0 " + std::to_string(pow(tan(SemiAngle), 2.0)) + " ";
            if (cone_Dir.IsOpposite(gp::DX(), angularTolerance))
            {
                surfExpr += boost::str(boost::format("%13.7f")
                    %-1);
                coeffs += "-1";
            }
            else {
                surfExpr += boost::str(boost::format("%13.7f")
                    %1);
                coeffs += "1";
            }
        }
        else {
            coneSurface->accessSImpl()->surfSymb = "K/X";
            surfExpr += boost::str(boost::format("KX %13.7f  %13.7f  %13.7f  %13.7f")
                % X % Y % Z
                % pow(tan(SemiAngle), 2));
            coeffs = std::to_string(X) + " " + std::to_string(Y) +" "+ std::to_string(Z) + " " + std::to_string(pow(tan(SemiAngle), 2.0)) + " ";
            if (cone_Dir.IsOpposite(gp::DX(), angularTolerance))
            {
                surfExpr += boost::str(boost::format("%13.7f")
                    %-1);
                coeffs += "-1";
            }
            else {
                surfExpr += boost::str(boost::format("%13.7f")
                    %1);
                coeffs += "1";
            }
        }
        coneSurface->accessSImpl()->surfMcxPar = std::make_tuple("cone-x", coeffs.c_str(), "transmission");
    }
    else if(std::abs(coneAxisDir.X()) < precision && std::abs(coneAxisDir.Z()) < precision) {
        //for necp-mcx surface expression
        std::string coeffs;
        //cone parallel to Y-axis.
        if (abs(coneLocation.Y()) >= precision && abs(coneLocation.X()) <= precision && abs(coneLocation.Z()) <= precision) {
            coneSurface->accessSImpl()->surfSymb = "KY";
            surfExpr += boost::str(boost::format("KY %13.7f  %13.7f")
                % Y
                % pow(tan(SemiAngle), 2.0));
            coeffs = "0.0 "+std::to_string(Y) + " 0.0 " + std::to_string(pow(tan(SemiAngle), 2.0)) + " ";
            if (cone_Dir.IsOpposite(gp::DY(), angularTolerance))
            {
                surfExpr += boost::str(boost::format("%13.7f")
                    %-1);
                coeffs += "-1";
            }
            else {
                surfExpr += boost::str(boost::format("%13.7f")
                    %1);
                coeffs += "1";
            }
        }
        else {
            coneSurface->accessSImpl()->surfSymb = "K/Y";
            surfExpr += boost::str(boost::format("KY %13.7f  %13.7f  %13.7f  %13.7f")
                % X % Y % Z
                % pow(tan(SemiAngle), 2));
            coeffs = std::to_string(X) + std::to_string(Y) + std::to_string(Z) + " " + std::to_string(pow(tan(SemiAngle), 2.0)) + " ";
            if (cone_Dir.IsOpposite(gp::DY(), angularTolerance))
            {
                surfExpr += boost::str(boost::format("%13.7f")
                    %-1);
                coeffs += "-1";
            }
            else {
                surfExpr += boost::str(boost::format("%13.7f")
                    %1);
                coeffs += "1";

            }
        }
        coneSurface->accessSImpl()->surfMcxPar = std::make_tuple("cone-y", coeffs.c_str(), "transmission");
    }
    else if (std::abs(coneAxisDir.X()) < precision && std::abs(coneAxisDir.Y()) < precision) {
        //for necp-mcx surface expression
        std::string coeffs;
        //cone parallel to  Z-axis.
        if (abs(coneLocation.Z()) >= precision && abs(coneLocation.X()) <= precision && abs(coneLocation.Y()) <= precision) {
            coneSurface->accessSImpl()->surfSymb = "KZ";
                surfExpr += boost::str(boost::format("KZ %13.7f  %13.7f")
                    % Z
                    % pow(tan(SemiAngle), 2.0));
                coeffs = "0.0 0.0 "+ std::to_string(Z) + " " + std::to_string(pow(tan(SemiAngle), 2.0)) + " ";
                if (cone_Dir.IsOpposite(gp::DZ(), angularTolerance))
                {
                    surfExpr += boost::str(boost::format("%13.7f")
                        %-1);
                    coeffs += "-1";
                }
                else {
                    surfExpr += boost::str(boost::format("%13.7f")
                        %1);
                    coeffs += "1";

                }

        }
        else {
            coneSurface->accessSImpl()->surfSymb = "K/Z";
                surfExpr += boost::str(boost::format("KY %13.7f  %13.7f  %13.7f  %13.7f")
                    % X % Y % Z
                    % pow(tan(SemiAngle), 2));
                coeffs = std::to_string(X) + std::to_string(Y) + std::to_string(Z) + " " + std::to_string(pow(tan(SemiAngle), 2.0)) + " ";
                if (cone_Dir.IsOpposite(gp::DZ(), angularTolerance))
                {
                    surfExpr += boost::str(boost::format("%13.7f")
                        %-1);
                    coeffs += "-1";
                }
                else {
                    surfExpr += boost::str(boost::format("%13.7f")
                        %1);
                    coeffs += "1";
                }
        }
        coneSurface->accessSImpl()->surfMcxPar = std::make_tuple("cone-z", coeffs.c_str(), "transmission");
    }
    else {
        double parmtA{ coneSurface->accessSImpl()->surfParameters[0] },
            parmtB{ coneSurface->accessSImpl()->surfParameters[1] },
            parmtC{ coneSurface->accessSImpl()->surfParameters[2] },
            parmtD{ 2 * coneSurface->accessSImpl()->surfParameters[3] },
            parmtE{ 2 * coneSurface->accessSImpl()->surfParameters[5] },
            parmtF{ 2 * coneSurface->accessSImpl()->surfParameters[4] },
            parmtG{ 2 * coneSurface->accessSImpl()->surfParameters[6] },
            parmtH{ 2 * coneSurface->accessSImpl()->surfParameters[7] },
            parmtJ{ 2 * coneSurface->accessSImpl()->surfParameters[8] },
            parmtK{ coneSurface->accessSImpl()->surfParameters[9] };
        coneSurface->accessSImpl()->surfSymb = "GQ";
        surfExpr += boost::str(boost::format("GQ %13.7f  %13.7f  %13.7f %13.7f  %13.7f  "
            "%13.7f %13.7f  %13.7f  %13.7f  %13.7f")
            % parmtA % parmtB % parmtC % parmtD % parmtE
            % parmtF % parmtG % parmtH % parmtJ % parmtK);
        //for necp-mcx surface expression
        std::string coeffs = std::to_string(parmtA) + " " +
            std::to_string(parmtB) + " " +
            std::to_string(parmtC) + " " +
            std::to_string(parmtD) + " " +
            std::to_string(parmtE) + " " +
            std::to_string(parmtF) + " " +
            std::to_string(parmtG) + " " +
            std::to_string(parmtH) + " " +
            std::to_string(parmtJ) + " " +
            std::to_string(parmtK) + " ";
        coneSurface->accessSImpl()->surfMcxPar = std::make_tuple("gq", coeffs.c_str(), "transmission");
    }
    coneSurface->accessSImpl()->surfExpr = surfExpr;
}

/** ********************************************************************
* @brief   The function adds the surfaces of the solid to a list to be used for cell expression generation.
* @param   solidObj is a solid object.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::createSurfacesList(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    for (const auto& surface : solidObj->accessSImpl()->facesList){
        solidObj->accessSImpl()->intersectionList.push_back(surface);
    }
    for (const auto& assistSurface : solidObj->accessSImpl()->assistFacesList){
        if (assistSurface->accessSImpl()->splitSurface){
            solidObj->accessSImpl()->unionList.push_back(assistSurface);
        } else{
            solidObj->accessSImpl()->intersectionList.push_back(assistSurface);
        }
    }
}

/** ********************************************************************
* @brief   The function genrates the cell expression of the material cells.
* @param   solidObj is a solid object.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::genCellExpr(
        const std::shared_ptr<Geometry::Solid>& solidObj){
    std::string cellExpr, complimentExpr;
    std::string cellMcxExpr, complimentMcxExpr;
    complimentExpr += " (";
    complimentMcxExpr += " (";
    for(const auto& surface : solidObj->accessSImpl()->intersectionList){
        auto surfaceIDSigned = surface->accessSImpl()->surfSense *
                surface->accessSImpl()->uniqueID;
        cellExpr += boost::str(boost::format("%d ") % surfaceIDSigned);
        cellMcxExpr += boost::str(boost::format("%d ") % surfaceIDSigned);
        complimentExpr += boost::str(boost::format("%d : ") % (-1*surfaceIDSigned));
        complimentMcxExpr += boost::str(boost::format("%d | ") % (-1 * surfaceIDSigned));
    }
    cellExpr.resize(cellExpr.size() - 1);             // Remove the last space.
    complimentExpr.resize(complimentExpr.size() - 3); // Remove the last three spaces.
    cellMcxExpr.resize(cellMcxExpr.size() - 1);             // Remove the last space.
    complimentMcxExpr.resize(complimentMcxExpr.size() - 3); // Remove the last three spaces.
    complimentExpr += ")";
    complimentMcxExpr += ")";
    if(solidObj->accessSImpl()->unionList.size() > 1){
        cellExpr += " (";
        cellMcxExpr  += " (";
        for(const auto& surface : solidObj->accessSImpl()->unionList){
            auto surfaceIDSigned = surface->accessSImpl()->surfSense *
                    surface->accessSImpl()->uniqueID;
            cellExpr += boost::str(boost::format("%d : ") % surfaceIDSigned);
            cellMcxExpr += boost::str(boost::format("%d | ") % surfaceIDSigned);
            complimentExpr += boost::str(boost::format("%d ") % (-1*surfaceIDSigned));
            complimentMcxExpr += boost::str(boost::format("%d ") % (-1 * surfaceIDSigned));
        }
        cellExpr.resize(cellExpr.size() - 3);     // Remove the last three spaces.
        cellExpr += ")";                          // Remove the last space.
        complimentExpr.resize(complimentExpr.size() - 1);

        cellMcxExpr.resize(cellMcxExpr.size() - 3);
        cellMcxExpr += ")";
        complimentMcxExpr.resize(complimentMcxExpr.size() - 1);
    }
    solidObj->accessSImpl()->cellExpr = cellExpr;
    solidObj->accessSImpl()->cellMcxExpr = cellMcxExpr;
    solidObj->accessSImpl()->complimentExpr = complimentExpr;
    solidObj->accessSImpl()->complimentMcxExpr = complimentMcxExpr;
}

/** ********************************************************************
* @brief   The function genrates the surface expression of the void cells.
* @param   voidCell is a void cell to generate the surface expressions for.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPExprGenerator::genVoidExpr(const std::shared_ptr<VoidCell>& voidCell){
    std::string voidSurfExpr;
    voidSurfExpr = boost::str(boost::format("RPP %13.7f %13.7f %13.7f %13.7f %13.7f %13.7f")
                          % (voidCell->minX * scalingFactor) % (voidCell->maxX * scalingFactor)
                          % (voidCell->minY * scalingFactor) % (voidCell->maxY * scalingFactor)
                          % (voidCell->minZ * scalingFactor) % (voidCell->maxZ * scalingFactor));
    voidCell->voidSurfExpr = voidSurfExpr;
    //using for mcx input file
    std::string coeffs =
        std::to_string(voidCell->minX * scalingFactor) + " " +
        std::to_string(voidCell->maxX * scalingFactor) + " " +
        std::to_string(voidCell->minY * scalingFactor) + " " +
        std::to_string(voidCell->maxY * scalingFactor) + " " +
        std::to_string(voidCell->minZ * scalingFactor) + " " +
        std::to_string(voidCell->maxZ * scalingFactor);
        voidCell->voidParMap = std::make_tuple("box", coeffs.c_str(), "reflect");
}
