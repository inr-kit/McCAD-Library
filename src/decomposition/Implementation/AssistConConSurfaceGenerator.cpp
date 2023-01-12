//C++
#include <math.h>
// McCAD
#include "AssistConConSurfaceGenerator.hpp"
#include "CommonEdgeFinder.hpp"
#include "EdgesCombiner.hpp"
#include "CurveUtilities.hpp"
#include "splitSurfacesGenerator.hpp"
#include "surfaceObjCerator.hpp"
// OCCT
#include <GeomAbs_CurveType.hxx>
#include <STEPControl_Writer.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRep_Tool.hxx>

McCAD::Decomposition::AssistConConSurfaceGenerator::AssistConConSurfaceGenerator(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{
}

McCAD::Decomposition::AssistConConSurfaceGenerator::~AssistConConSurfaceGenerator(){
}

/** ********************************************************************
* @brief    Operator that performs main operations for assist surface generation.
* @param    solidObj is a McCAD conical solid object.
* @return
* @date     09/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Decomposition::AssistConConSurfaceGenerator::operator()(
        Geometry::CONSolid& solidObj){
    auto& conesList = solidObj.accessSImpl()->conesList;
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    edgesMap commonLineEdgesMap, commonCurveEdgesMap;
    for(int i = 0; i < conesList.size() - 1; ++i){
        for(int j = i+1; j < conesList.size(); ++j){
            if (*conesList[i] == *conesList[j]) continue;
            commonEdges = CommonEdgeFinder{inputConfig.angularTolerance,
                    inputConfig.distanceTolerance, inputConfig.precision}(
                        conesList[i], conesList[j]);
            if(!commonEdges.empty()){
                for(int k = 0; k < commonEdges.size(); ++k){
                    // Only accept edges that are not spot or seem edges.
                    if(!BRep_Tool::IsClosed(commonEdges[k]->accessEImpl()->edge,
                                            conesList[i]->accessSImpl()->face) &&
                       !BRep_Tool::Degenerated(commonEdges[k]->accessEImpl()->edge)){
                        if(commonEdges[k]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line))
                            commonLineEdgesMap[j].push_back(commonEdges[k]);
                        else commonCurveEdgesMap[j].push_back(commonEdges[k]);
                    }
                }
            }
        }
        if(!commonLineEdgesMap.empty()){
            // Cone has common line edges with other cone(s).
            if (commonLineEdgesMap.size() == 1){
                // Cone has common line edges with a single cone.
                for(const auto& member : commonLineEdgesMap){
                    if(member.second.size() == 1){
                        auto assistSurface = generateThroughLine(
                                    conesList[i], conesList[member.first],
                                    member.second[0], solidObj.accessSImpl()->boxDiagonalLength,
                                    solidObj.accessSImpl()->meshDeflection);
                        if(assistSurface){
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[conesList[i]] = assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[conesList[member.first]] = assistSurface.value();
                        } else{
                            // If there exists a common edge between conical surfaces,
                            // but failed to generate a split surface then reject the solid.
                            solidObj.accessSImpl()->rejectSolid = true;
                        }
                    } else if (member.second.size() == 2){
                        auto assistSurface = generateThroughTwoLines(
                                    conesList[i], member.second[0], member.second[1],
                                    solidObj.accessSImpl()->boxDiagonalLength,
                                    solidObj.accessSImpl()->meshDeflection);
                        if(assistSurface){
                            // Set solid flags.
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[conesList[i]]= assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[conesList[member.first]]= assistSurface.value();
                        } else{
                            // If there exists a common edge between conical surfaces,
                            // but failed to generate a split surface then reject solid.
                            solidObj.accessSImpl()->rejectSolid = true;
                        }
                    }
                }
            } else if (commonLineEdgesMap.size() == 2){
                // Cone has common edges with two cones.
                std::vector<std::shared_ptr<Geometry::Edge>> commonEdgesToUse;
                for(const auto& member : commonLineEdgesMap){
                    if(member.second.size() == 1) commonEdgesToUse.push_back(member.second[0]);
                }
                if(commonEdgesToUse.size() == 2){
                    auto assistSurface = generateThroughTwoLines(
                            conesList[i], commonEdgesToUse[0], commonEdgesToUse[1],
                            solidObj.accessSImpl()->boxDiagonalLength,
                            solidObj.accessSImpl()->meshDeflection);
                    if(assistSurface){
                        solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[conesList[i]] = assistSurface.value();
                    } else{
                    // If there exists common edges between conical surfaces
                    // but failed to generate a split surface then reject solid.
                    solidObj.accessSImpl()->rejectSolid = true;
                    }
                }
            }
        }
        if(!commonCurveEdgesMap.empty()){
            // Generate surface through curved edge; circle, ellipse, parabola, hyperabola.
            if (commonCurveEdgesMap.size() == 1){
                for(const auto& member : commonCurveEdgesMap){
                    if(member.second.size() == 1){
                        auto assistSurface = generateThroughCurve(
                                    conesList[i], conesList[member.first],
                                member.second[0], solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                        if(assistSurface){
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[conesList[i]] = assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[conesList[member.first]] = assistSurface.value();
                        } else{
                            // If there exists a common edge between cylindrical surfaces,
                            // but failed to generate a split surface then reject solid.
                            solidObj.accessSImpl()->rejectSolid = true;
                        }
                    }
                }
            }
        }
        commonLineEdgesMap.clear();
        commonCurveEdgesMap.clear();
    }
}

/** ********************************************************************
* @brief    A function that creates a split surface through a line edge.
* @param    firstFace and secondFace are McCAD bound surface objects.
* @param    commonEdge is a McCAD edge object.
* @param    boxDiagonalLength is the diagonal of the created AABB of the conical solid.
* @param    meshDeflection is the calculated mesh size for the conical solid.
* @return   an optional McCAD bound surface object.
* @date     09/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistConConSurfaceGenerator::generateThroughLine(
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
* @param    boxDiagonalLength is the diagonal of the created AABB of the conical solid.
* @param    meshDeflection is the calculated mesh size for the conical solid.
* @return   an optional McCAD bound surface object.
* @date     09/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistConConSurfaceGenerator::generateThroughCurve(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
        const std::shared_ptr<Geometry::Edge>& commonEdge,
        const double& boxDiagonalLength, const double& meshDeflection){
    std::optional<TopoDS_Face> splitFace;
    BRepAdaptor_Curve curveAdaptor(commonEdge->accessEImpl()->edge);
    if (curveAdaptor.GetType() == GeomAbs_BSplineCurve){
        splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
                inputConfig.precision, inputConfig.angularTolerance}.generateSurfOnBSpline(
                    firstFace->accessSImpl()->face, secondFace->accessSImpl()->face,
                    commonEdge);
    } else if (curveAdaptor.GetType() == GeomAbs_Circle ||
               curveAdaptor.GetType() == GeomAbs_Ellipse ||
               curveAdaptor.GetType() == GeomAbs_Parabola ||
               curveAdaptor.GetType() == GeomAbs_Hyperbola){
        splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
                inputConfig.precision, inputConfig.angularTolerance}.generatePlaneOnCurve(commonEdge);
    } else splitFace = std::nullopt;
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
* @param    firstFace is McCAD bound surface object.
* @param    firstEdge and secondEdge are McCAD edge objects.
* @param    boxDiagonalLength is the diagonal of the created AABB of the conical solid.
* @param    meshDeflection is the calculated mesh size for the conical solid.
* @return   an optional McCAD bound surface object.
* @date     09/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistConConSurfaceGenerator::generateThroughTwoLines(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
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
            // Edges are not planar!. Cannot be used to vreate surface.
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
                firstFace->accessSImpl()->surfaceNumber * 2000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)){
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(firstEdge);
        assistSurface->accessBSImpl()->assistEdgesList.push_back(secondEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 2;
        assistSurface->accessSImpl()->isAssistSurface = true;
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = true;
        firstEdge->accessEImpl()->useForSplitSurface = true;
        secondEdge->accessEImpl()->useForSplitSurface = true;
        return assistSurface;
    }
    return std::nullopt;
}
