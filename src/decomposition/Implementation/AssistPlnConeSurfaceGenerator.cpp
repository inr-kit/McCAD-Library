//C++
#include <array>
#include <filesystem>
// McCAD
#include "AssistPlnConeSurfaceGenerator.hpp"
#include "CommonEdgeFinder.hpp"
#include "CurveUtilities.hpp"
#include "splitSurfacesGenerator.hpp"
#include "surfaceObjCerator.hpp"
#include "faceParameters.hpp"
//OCC
#include <GeomAbs_CurveType.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <TopAbs_Orientation.hxx>
#include <BRepAdaptor_Surface.hxx>
//#include <STEPControl_Writer.hxx>

McCAD::Decomposition::AssistPlnConeSurfaceGenerator::AssistPlnConeSurfaceGenerator(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{
}

McCAD::Decomposition::AssistPlnConeSurfaceGenerator::~AssistPlnConeSurfaceGenerator(){
}

/** ********************************************************************
* @brief    Operator that performs main operations for assist surface generation.
* @param    solidObj is a McCAD conical solid object.
* @return   
* @date     04/11/2022
* @modified 
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Decomposition::AssistPlnConeSurfaceGenerator::operator()(Geometry::CONSolid& solidObj) {
    auto& conesList = solidObj.accessSImpl()->conesList;
    auto& planesList = solidObj.accessSImpl()->planesList;
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    edgesMap commonLineEdgesMap, commonCurveEdgesMap;
    for (int i = 0; i < conesList.size(); ++i) {
        // If cone is closed, then ignore the conical surface.
        // Commenting out the radian check since all cones have U1=0 and U2=2PI.
        //if (Tools::FaceParameters{}.getRadian(conesList[i]->accessSImpl()->face) >= 2 * inputConfig.PI) continue;
        for (int j = 0; j < planesList.size(); ++j) {
            commonEdges = CommonEdgeFinder{inputConfig.angularTolerance,
                    inputConfig.distanceTolerance, inputConfig.precision }(
                        conesList[i], planesList[j]);
            if (!commonEdges.empty()) {
                for (int k = 0; k < commonEdges.size(); ++k) {
                    // Only accept edges that are not spot or seem edges
                    if (!BRep_Tool::IsClosed(commonEdges[k]->accessEImpl()->edge,
                        conesList[i]->accessSImpl()->face) &&
                        !BRep_Tool::Degenerated(commonEdges[k]->accessEImpl()->edge)) {
                        if (commonEdges[k]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line))
                            commonLineEdgesMap[j].push_back(commonEdges[k]);
                        else commonCurveEdgesMap[j].push_back(commonEdges[k]);
                    }
                }
            }
        }
        if (!commonLineEdgesMap.empty()) {
            // The Cone has common line edges with planes.
            if (commonLineEdgesMap.size() == 1) {
                // The Cone has common line edges with a single plane.
                for (const auto& member : commonLineEdgesMap) {
                    //if(!planesList[member.first]->accessSImpl()->splitSurface) continue;
                    if (member.second.size() == 1) {
                        // The cone has a single common line edge with a single plane.
                        auto assistSurface = generateThroughLineAxis(
                            conesList[i], member.second[0],
                            solidObj.accessSImpl()->boxDiagonalLength,
                            solidObj.accessSImpl()->meshDeflection);
                        if (assistSurface) {
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[conesList[i]] = assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[planesList[member.first]] = assistSurface.value();
                        }
                        else {
                            // If there exists a common edge between conical surfaces,
                            // but failed to generate a split surface then reject solid.
                            solidObj.accessSImpl()->rejectSolid = true;
                        }
                    }
                }
            }
            else if (commonLineEdgesMap.size() == 2) {
                std::vector<std::shared_ptr<Geometry::Edge>> commonEdgesToUse;
                for (const auto& member : commonLineEdgesMap) {
                    if (member.second.size() == 1) {
                        commonEdgesToUse.push_back(member.second[0]);
                    }
                }
                if (conesList[i]->accessSImpl()->face.Orientation() == TopAbs_REVERSED) {
                    // Cone is concave.
                    goto useBothEdges;
                }
                else if (conesList[i]->accessSImpl()->face.Orientation() == TopAbs_FORWARD) {
                    // Cone is convex.
                    if (Tools::FaceParameters{}.getRadian(conesList[i]->accessSImpl()->face) > 3 * inputConfig.PI / 4.0) {
                    useBothEdges:;
                        // Generate split surface through the two edges.
                        if (commonEdgesToUse.size() == 2) {
                            auto assistSurface = generateThroughTwoLines(
                                conesList[i], commonEdgesToUse[0], commonEdgesToUse[1],
                                solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                            if (assistSurface) {
                                solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                                solidObj.accessSImpl()->assistFacesMap[conesList[i]] = assistSurface.value();
                            }
                            else {
                                // If there exists a common edge between conical surfaces,
                                // but failed to generate a split surface then reject solid.
                                solidObj.accessSImpl()->rejectSolid = true;
                            }
                        }
                    }
                    else {
                        // Generate split surfaces through the axis and the edges.
                        int increment{ 0 };
                        if (commonEdgesToUse.size() >= 1) {
                            for (const auto& edge : commonEdgesToUse) {
                                auto assistSurface = generateThroughLineAxis(
                                    conesList[i], edge,
                                    solidObj.accessSImpl()->boxDiagonalLength,
                                    solidObj.accessSImpl()->meshDeflection);
                                if (assistSurface) {
                                    assistSurface.value()->accessSImpl()->surfaceNumber += increment;
                                    solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                                    solidObj.accessSImpl()->assistFacesMap[conesList[i]] = assistSurface.value();
                                }
                                ++increment;
                            }
                            if (!solidObj.accessSImpl()->assistFacesMap[conesList[i]]) {
                                // If there exists a common edge between conical surfaces,
                                // but failed to generate a split surface then reject solid.
                                solidObj.accessSImpl()->rejectSolid = true;
                            }
                        }
                    }
                }
            }
        }
        if (!commonCurveEdgesMap.empty()) {
            // Cone has common curved edges with a plane.
        }
        commonLineEdgesMap.clear();
        commonCurveEdgesMap.clear();
    }
}

/** ********************************************************************
* @brief    A function that creates a split surface through a line edge and cone axis.
* @param    coneSurface is a McCAD bound surface object.
* @param    commonEdge is a McCAD edge object.
* @param    boxDiagonalLength is the diagonal of the created AABB of the conical solid.
* @param    meshDeflection is the calculated mesh size for the conical solid.
* @return   an optional McCAD bound surface object.
* @date     11/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistPlnConeSurfaceGenerator::generateThroughLineAxis(
    const std::shared_ptr<Geometry::BoundSurface>& coneSurface,
    const std::shared_ptr<Geometry::Edge>& commonEdge,
    const double& boxDiagonalLength, const double& meshDeflection) {
    BRepAdaptor_Surface surfaceAdaptor(coneSurface->accessSImpl()->face);
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
            inputConfig.precision, inputConfig.angularTolerance}.generatePlaneOnLineAxis(
                surfaceAdaptor.Cone(), commonEdge);
    if (splitFace) {
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
            SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
            coneSurface->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)) {
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(commonEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 1;
        assistSurface->accessSImpl()->isAssistSurface = true;
        // Set the assist surface reference to the original surfaces.
        coneSurface->accessSImpl()->hasAssistSurface = true;
        commonEdge->accessEImpl()->useForSplitSurface = true;
        return assistSurface;
    }
    return std::nullopt;
}

/** ********************************************************************
* @brief    A function that creates a split surface through two line edges.
* @param    coneSurface is a McCAD bound surface object.
* @param    firstEdge and secondEdge are McCAD edge objects.
* @param    boxDiagonalLength is the diagonal of the created AABB of the conical solid.
* @param    meshDeflection is the calculated mesh size for the conical solid.
* @return   an optional McCAD bound surface object.
* @date     11/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistPlnConeSurfaceGenerator::generateThroughTwoLines(
    const std::shared_ptr<Geometry::BoundSurface>& coneSurface,
    const std::shared_ptr<Geometry::Edge>& firstEdge,
    const std::shared_ptr<Geometry::Edge>& secondEdge,
    const double& boxDiagonalLength, const double& meshDeflection) {
    // Need first to assert that the edges are planar.
    gp_Vec firstVec(firstEdge->accessEImpl()->startPoint,
                    firstEdge->accessEImpl()->endPoint),
           secondVec(secondEdge->accessEImpl()->startPoint,
                     secondEdge->accessEImpl()->endPoint);
    gp_Dir firstDir(firstVec), secondDir(secondVec);
    if (!firstDir.IsParallel(secondDir, inputConfig.angularTolerance) &&
        !firstDir.IsOpposite(secondDir, inputConfig.angularTolerance)) {
        // There is an angle between the two directions, Check if planar.
        gp_Dir normalDir{ firstDir.Crossed(secondDir) };
        if (!normalDir.IsNormal(firstDir, inputConfig.angularTolerance) &&
            !normalDir.IsNormal(secondDir, inputConfig.angularTolerance)) {
            // Edges are not co-planar!. Cannot be used to create a split surface.
            return std::nullopt;
        }
    }
    auto splitFace = SplitSurfaceGenerator{ inputConfig.edgeTolerance,
            inputConfig.precision, inputConfig.angularTolerance }.generatePlaneOn2Lines(
                firstEdge, secondEdge);
    if (splitFace) {
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
            SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
            coneSurface->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)) {
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(firstEdge);
        assistSurface->accessBSImpl()->assistEdgesList.push_back(secondEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 2;
        assistSurface->accessSImpl()->isAssistSurface = true;
        // Set the assist surface reference to the original surfaces.
        coneSurface->accessSImpl()->hasAssistSurface = true;
        firstEdge->accessEImpl()->useForSplitSurface = true;
        secondEdge->accessEImpl()->useForSplitSurface = true;
        return assistSurface;
    }
    return std::nullopt;
}
