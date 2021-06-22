//C++
#include <math.h>
#include <vector>
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
    for(Standard_Integer i = 0; i < cylindersList.size(); ++i){
        for(Standard_Integer j = i+1; j < cylindersList.size(); ++j){
            if (*cylindersList[i] == *cylindersList[j]) continue;
            commonEdges = CommonEdgeFinder{inputConfig.angularTolerance,
                    inputConfig.distanceTolerance, inputConfig.precision}(
                        cylindersList[i], cylindersList[j]);
            if(commonEdges.size() == 1){
                if(commonEdges[0]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
                    auto assistSurface = generateThroughLine(
                                cylindersList[i], cylindersList[j], commonEdges[0],
                                solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                    if(assistSurface){
                        solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[j]] = assistSurface.value();
                    }
                } else{
                    // Generate surface through curved edge; circle, ellipse, parabola, hyperabola.
                    auto assistSurface = generateThroughCurve(
                                cylindersList[i], cylindersList[j], commonEdges[0],
                                solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                    if(assistSurface){
                        solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[j]] = assistSurface.value();
                    }
                }
            } else if (commonEdges.size() == 2){
                if(commonEdges[0]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)
                   && commonEdges[1]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
                    auto assistSurface = generateThroughTwoLines(
                                cylindersList[i], cylindersList[j], commonEdges[0],
                                commonEdges[1], solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                    if(assistSurface){
                        solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]]= assistSurface.value();
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[j]]= assistSurface.value();
                    }
                }
            }
        }
    }
}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistCylCylSurfaceGenerator::generateThroughLine(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
        const std::shared_ptr<Geometry::Edge>& commonEdge,
        const Standard_Real& boxDiagonalLength, const Standard_Real& meshDeflection){
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
            inputConfig.precision}.generatePlaneOnLine(
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
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
            inputConfig.precision}.generatePlaneOnCurve(commonEdge);
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
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
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
            inputConfig.precision}.generatePlaneOn2Lines(firstEdge, secondEdge);
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
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = Standard_True;
        secondFace->accessSImpl()->hasAssistSurface = Standard_True;
        firstEdge->accessEImpl()->useForSplitSurface = Standard_True;
        secondEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}
