//C++
#include <math.h>
// McCAD
#include "AssistCylCylSurfaceGenerator.hpp"
#include "CommonEdgeFinder.hpp"
#include "EdgesCombiner.hpp"
#include "CurveUtilities.hpp"
#include "splitSurfacesGenerator.hpp"
#include "surfaceObjCerator.hpp"
//OCC
#include <GeomAbs_CurveType.hxx>
#include <STEPControl_Writer.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRep_Tool.hxx>

McCAD::Decomposition::AssistCylCylSurfaceGenerator::AssistCylCylSurfaceGenerator(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{
}

McCAD::Decomposition::AssistCylCylSurfaceGenerator::~AssistCylCylSurfaceGenerator(){
}

void
McCAD::Decomposition::AssistCylCylSurfaceGenerator::operator()(
        Geometry::CYLSolid& solidObj){
    auto& cylindersList = solidObj.accessSImpl()->cylindersList;
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    edgesMap commonLineEdgesMap, commonCurveEdgesMap;
    for(Standard_Integer i = 0; i < cylindersList.size() - 1; ++i){
        for(Standard_Integer j = i+1; j < cylindersList.size(); ++j){
            if (*cylindersList[i] == *cylindersList[j]) continue;
            commonEdges = CommonEdgeFinder{inputConfig.angularTolerance,
                    inputConfig.distanceTolerance, inputConfig.precision}(
                        cylindersList[i], cylindersList[j]);
            if(!commonEdges.empty()){
                for(Standard_Integer k = 0; k < commonEdges.size(); ++k){
                    if(!BRep_Tool::IsClosed(commonEdges[k]->accessEImpl()->edge,
                                            cylindersList[i]->accessSImpl()->face) &&
                       !BRep_Tool::Degenerated(commonEdges[k]->accessEImpl()->edge)){
                        if(commonEdges[k]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line))
                            commonLineEdgesMap[j].push_back(commonEdges[k]);
                        else commonCurveEdgesMap[j].push_back(commonEdges[k]);
                    }
                }
            }
        }
        if(!commonLineEdgesMap.empty()){
            // Cylinder has common line edges with other cylinders.
            if (commonLineEdgesMap.size() == 1){
                // Cylinder has common edges with a single cylinder.
                for(const auto& member : commonLineEdgesMap){
                    if(member.second.size() == 1){
                        auto assistSurface = generateThroughLine(
                                    cylindersList[i], cylindersList[member.first],
                                    member.second[0], solidObj.accessSImpl()->boxDiagonalLength,
                                    solidObj.accessSImpl()->meshDeflection);
                        if(assistSurface){
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[cylindersList[member.first]] = assistSurface.value();
                        } else{
                            // If there exists a common edge between cylindrical surfaces,
                            // but failed to generate a split surface then reject solid.
                            solidObj.accessSImpl()->rejectSolid = Standard_True;
                        }
                    } else if (member.second.size() == 2){
                        auto assistSurface = generateThroughTwoLines(
                                    cylindersList[i], member.second[0], member.second[1],
                                    solidObj.accessSImpl()->boxDiagonalLength,
                                    solidObj.accessSImpl()->meshDeflection);
                        if(assistSurface){
                            // Set solid flags.
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[cylindersList[i]]= assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[cylindersList[member.first]]= assistSurface.value();
                        } else{
                            // If there exists a common edge between cylindrical surfaces,
                            // but failed to generate a split surface then reject solid.
                            solidObj.accessSImpl()->rejectSolid = Standard_True;
                        }
                    }
                }
            } else if (commonLineEdgesMap.size() == 2){
                // // Cylinder has common edges with more than one cylinder.
                std::vector<std::shared_ptr<Geometry::Edge>> commonEdgesToUse;
                for(const auto& member : commonLineEdgesMap){
                    if(member.second.size() == 1) commonEdgesToUse.push_back(member.second[0]);
                }
                if(commonEdgesToUse.size() == 2){
                    auto assistSurface = generateThroughTwoLines(
                                cylindersList[i], commonEdgesToUse[0], commonEdgesToUse[1],
                            solidObj.accessSImpl()->boxDiagonalLength,
                            solidObj.accessSImpl()->meshDeflection);
                    if(assistSurface){
                        solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                    } else{
                    // If there exists a common edge between cylindrical surfaces,
                    // but failed to generate a split surface then reject solid.
                    solidObj.accessSImpl()->rejectSolid = Standard_True;
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
                                    cylindersList[i], cylindersList[member.first],
                                member.second[0], solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                        if(assistSurface){
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[cylindersList[member.first]] = assistSurface.value();
                        } else{
                            // If there exists a common edge between cylindrical surfaces,
                            // but failed to generate a split surface then reject solid.
                            solidObj.accessSImpl()->rejectSolid = Standard_True;
                        }
                    }
                }
            }
        }
        commonLineEdgesMap.clear();
        commonCurveEdgesMap.clear();
    }
}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistCylCylSurfaceGenerator::generateThroughLine(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
        const std::shared_ptr<Geometry::Edge>& commonEdge,
        const Standard_Real& boxDiagonalLength, const Standard_Real& meshDeflection){
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
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = Standard_True;
        secondFace->accessSImpl()->hasAssistSurface = Standard_True;
        commonEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistCylCylSurfaceGenerator::generateThroughCurve(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
        const std::shared_ptr<Geometry::Edge>& commonEdge,
        const Standard_Real& boxDiagonalLength, const Standard_Real& meshDeflection){
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
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = Standard_True;
        secondFace->accessSImpl()->hasAssistSurface = Standard_True;
        commonEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistCylCylSurfaceGenerator::generateThroughTwoLines(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::Edge>& firstEdge,
        const std::shared_ptr<Geometry::Edge>& secondEdge,
        const Standard_Real& boxDiagonalLength, const Standard_Real& meshDeflection){
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
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = Standard_True;
        firstEdge->accessEImpl()->useForSplitSurface = Standard_True;
        secondEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}
