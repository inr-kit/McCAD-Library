//C++
#include <math.h>
#include <vector>
// McCAD
#include "AssistCylConSurfaceGenerator.hpp"
#include "CommonEdgeFinder.hpp"
#include "EdgesCombiner.hpp"
#include "CurveUtilities.hpp"
#include "splitSurfacesGenerator.hpp"
#include "surfaceObjCerator.hpp"
// OCCT
#include <GeomAbs_CurveType.hxx>

McCAD::Decomposition::AssistCylConSurfaceGenerator::AssistCylConSurfaceGenerator(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{
}

McCAD::Decomposition::AssistCylConSurfaceGenerator::~AssistCylConSurfaceGenerator(){
}

/** ********************************************************************
* @brief    Operator that performs main operations for assist surface generation.
* @param    solidObj is a McCAD mixed solid object.
* @return
* @date     08/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Decomposition::AssistCylConSurfaceGenerator::operator()(Geometry::MXDSolid& solidObj){
    auto& cylindersList = solidObj.accessSImpl()->cylindersList;
    auto& conesList = solidObj.accessSImpl()->conesList;
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    for(int i = 0; i < cylindersList.size(); ++i){
        for(int j = 0; j < conesList.size(); ++j){
            commonEdges = CommonEdgeFinder{inputConfig.angularTolerance,
                    inputConfig.distanceTolerance, inputConfig.precision}(
                        cylindersList[i], conesList[j]);
            if(commonEdges.size() == 1){
                if(commonEdges[0]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
                    auto assistSurface = generateThroughLine(
                                cylindersList[i], conesList[j], commonEdges[0],
                                solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                    if(assistSurface){
                        solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                        solidObj.accessSImpl()->assistFacesMap[conesList[j]] = assistSurface.value();
                    } else{
                    // If there exists a common edge between cylindrical and tori surfaces,
                    // but failed to generate a split surface then reject solid.
                    solidObj.accessSImpl()->rejectSolid = true;
                    }
                } else{
                    // Generate surface through curved edge; circle, ellipse, parabola, hyperabola.
                    auto assistSurface = generateThroughCurve(
                                cylindersList[i], conesList[j], commonEdges[0],
                                solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                    if(assistSurface){
                        solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                        solidObj.accessSImpl()->assistFacesMap[conesList[j]] = assistSurface.value();
                    } else{
                        // If there exists a common edge between cylindrical and tori surfaces,
                        // but failed to generate a split surface then reject solid.
                        solidObj.accessSImpl()->rejectSolid = true;
                    }
                }
            } else if (commonEdges.size() == 2){
                if(commonEdges[0]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)
                   && commonEdges[1]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
                    auto assistSurface = generateThroughTwoLines(
                                cylindersList[i], conesList[j], commonEdges[0],
                                commonEdges[1], solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                    if(assistSurface){
                        solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]]= assistSurface.value();
                        solidObj.accessSImpl()->assistFacesMap[conesList[j]]= assistSurface.value();
                    } else{
                        // If there exists a common edge between cylindrical and tori surfaces,
                        // but failed to generate a split surface then reject solid.
                        solidObj.accessSImpl()->rejectSolid = true;
                    }
                }
            }
        }
    }
}

/** ********************************************************************
* @brief    A function that creates a split surface through a line edge.
* @param    coneSurface is a McCAD bound surface object.
* @param    commonEdge is a McCAD edge object.
* @param    boxDiagonalLength is the diagonal of the created AABB of the mixed solid.
* @param    meshDeflection is the calculated mesh size for the mixed solid.
* @return   an optional McCAD bound surface object.
* @date     08/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistCylConSurfaceGenerator::generateThroughLine(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
        const std::shared_ptr<Geometry::Edge>& commonEdge,
        const double& boxDiagonalLength, const double& meshDeflection){
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
            inputConfig.precision, inputConfig.angularTolerance}.generatePlaneOnLine(
                firstFace->accessSImpl()->face, secondFace->accessSImpl()->face,
                commonEdge);
    if(splitFace){
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
                SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                                    inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
                firstFace->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)){
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(commonEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 1;
        assistSurface->accessSImpl()->isAssistSurface = true;
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = true;
        secondFace->accessSImpl()->hasAssistSurface = true;
        commonEdge->accessEImpl()->useForSplitSurface = true;
        return assistSurface;
    }
    return std::nullopt;
}

/** ********************************************************************
* @brief    A function that creates a split surface through a curved edge.
* @param    firstFace and secondFace are McCAD bound surface objects.
* @param    commonEdge is a McCAD edge object.
* @param    boxDiagonalLength is the diagonal of the created AABB of the mixed solid.
* @param    meshDeflection is the calculated mesh size for the mixed solid.
* @return   an optional McCAD bound surface object.
* @date     08/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistCylConSurfaceGenerator::generateThroughCurve(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
        const std::shared_ptr<Geometry::Edge>& commonEdge,
        const double& boxDiagonalLength, const double& meshDeflection){
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
            inputConfig.precision, inputConfig.angularTolerance}.generatePlaneOnCurve(commonEdge);
    if(splitFace){
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
                SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                                    inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
                firstFace->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)){
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(commonEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 1;
        assistSurface->accessSImpl()->isAssistSurface = true;
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = true;
        secondFace->accessSImpl()->hasAssistSurface = true;
        commonEdge->accessEImpl()->useForSplitSurface = true;
        return assistSurface;
    }
    return std::nullopt;
}

/** ********************************************************************
* @brief    A function that creates a split surface through two line edges.
* @param    firstFace and secondFace are McCAD bound surface objects.
* @param    firstEdge and secondEdge are McCAD edge objects.
* @param    boxDiagonalLength is the diagonal of the created AABB of the mixed solid.
* @param    meshDeflection is the calculated mesh size for the mixed solid.
* @return   an optional McCAD bound surface object.
* @date     08/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistCylConSurfaceGenerator::generateThroughTwoLines(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
        const std::shared_ptr<Geometry::Edge>& firstEdge,
        const std::shared_ptr<Geometry::Edge>& secondEdge,
        const double& boxDiagonalLength, const double& meshDeflection){
    // Need first to assert that the edges are planar.
    gp_Vec firstVec(firstEdge->accessEImpl()->startPoint,
                    firstEdge->accessEImpl()->endPoint),
           secondVec(secondEdge->accessEImpl()->startPoint,
                     secondEdge->accessEImpl()->endPoint);
    gp_Dir firstDir(firstVec), secondDir(secondVec);
    if(!firstDir.IsParallel(secondDir, inputConfig.angularTolerance) &&
            !firstDir.IsOpposite(secondDir, inputConfig.angularTolerance)){
        // There is an angle between the two directions, Check if planar.
        gp_Dir normalDir{firstDir.Crossed(secondDir)};
        if(!normalDir.IsNormal(firstDir, inputConfig.angularTolerance) &&
           !normalDir.IsNormal(secondDir, inputConfig.angularTolerance)){
            // Edges are not planar!. Cannot be used to create surface.
            return std::nullopt;
        }
    }
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
            inputConfig.precision, inputConfig.angularTolerance}.generatePlaneOn2Lines(firstEdge, secondEdge);
    if(splitFace){
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
                SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                                    inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
                firstFace->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)){
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(firstEdge);
        assistSurface->accessBSImpl()->assistEdgesList.push_back(secondEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 2;
        assistSurface->accessSImpl()->isAssistSurface = true;
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = true;
        secondFace->accessSImpl()->hasAssistSurface = true;
        firstEdge->accessEImpl()->useForSplitSurface = true;
        secondEdge->accessEImpl()->useForSplitSurface = true;
        return assistSurface;
    }
    return std::nullopt;
}