//C++
#include <array>
#include <filesystem>
// McCAD
#include "AssistPlnCylSurfaceGenerator.hpp"
#include "CommonEdgeFinder.hpp"
#include "CurveUtilities.hpp"
#include "splitSurfacesGenerator.hpp"
#include "surfaceObjCerator.hpp"
//OCC
#include <GeomAbs_CurveType.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <TopAbs_Orientation.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <STEPControl_Writer.hxx>

McCAD::Decomposition::AssistPlnCylSurfaceGenerator::AssistPlnCylSurfaceGenerator(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{
}

McCAD::Decomposition::AssistPlnCylSurfaceGenerator::~AssistPlnCylSurfaceGenerator(){
}

void
McCAD::Decomposition::AssistPlnCylSurfaceGenerator::operator()(
        Geometry::CYLSolid& solidObj){
    auto& cylindersList = solidObj.accessSImpl()->cylindersList;
    auto& planesList = solidObj.accessSImpl()->planesList;
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    edgesMap commonLineEdgesMap, commonCurveEdgesMap;
    for(Standard_Integer i = 0; i < cylindersList.size(); ++i){
        if(!checkRadian(cylindersList[i]->accessSImpl()->face)) continue;
        for(Standard_Integer j = 0; j < planesList.size(); ++j){
            commonEdges = CommonEdgeFinder{inputConfig.angularTolerance,
                    inputConfig.distanceTolerance, inputConfig.precision}(
                        cylindersList[i], planesList[j]);
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
            // The Cylinder has common line edges with planes.
            if (commonLineEdgesMap.size() == 1){
                // The Cylinder has common line edges with a single plane.
                for(const auto& member : commonLineEdgesMap){
                    if(!planesList[member.first]->accessSImpl()->splitSurface) continue;
                    if(member.second.size() == 1){
                        auto assistSurface = generateThroughLineAxis(
                                    cylindersList[i], member.second[0],
                                solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                        if(assistSurface){
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[planesList[member.first]] = assistSurface.value();
                        } else{
                            // If there exists a common edge between cylindrical surfaces,
                            // but failed to generate a split surface then reject solid.
                            solidObj.accessSImpl()->rejectSolid = Standard_True;
                        }
                    }
                }
            } else if (commonLineEdgesMap.size() == 2){
                if(cylindersList[i]->accessSImpl()->face.Orientation() == TopAbs_FORWARD){
                    // Cylinder is convex.
                    goto treatAsConcave;
                } else if(cylindersList[i]->accessSImpl()->face.Orientation() == TopAbs_REVERSED){
                    treatAsConcave:;
                    // Cylinder is concave. Check that the edges are convex.
                    std::vector<std::shared_ptr<Geometry::Edge>> commonEdgesToUse;
                    for(const auto& member : commonLineEdgesMap){
                        if(member.second.size() == 1 &&
                                !planesList[member.first]->accessSImpl()->splitSurface &&
                                member.second[0]->accessEImpl()->edge.Convex() == 1){
                            commonEdgesToUse.push_back(member.second[0]);
                        }
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
        }
        if (!commonCurveEdgesMap.empty()){
            // Cylinder has common edges with more than one plane.
        }
        commonLineEdgesMap.clear();
        commonCurveEdgesMap.clear();
    }
}

Standard_Boolean
McCAD::Decomposition::AssistPlnCylSurfaceGenerator::checkRadian(const TopoDS_Face& cylinder){
    // Radian check for now returns True always!. This is to be modified later on
    // for a more specialized assist. surface generations.
    std::array<Standard_Real, 4> uvParameters;
    // UV parameters in class grom_Cylindrical_Surface: U1 = 0 and U2 = 2*PI.
    BRepTools::UVBounds(cylinder, uvParameters[0], uvParameters[1], uvParameters[2],
            uvParameters[3]);
    return std::abs(uvParameters[1] - uvParameters[0]) <= 2*inputConfig.PI ?
            Standard_True : Standard_False;
}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistPlnCylSurfaceGenerator::generateThroughLineAxis(
        const std::shared_ptr<Geometry::BoundSurface>& cylinderFace,
        const std::shared_ptr<Geometry::Edge>& commonEdge,
        const Standard_Real& boxDiagonalLength, const Standard_Real& meshDeflection){
    BRepAdaptor_Surface surfaceAdaptor(cylinderFace->accessSImpl()->face);
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance,
            inputConfig.precision, inputConfig.angularTolerance}.generatePlaneOnLineAxis(
                surfaceAdaptor.Cylinder(), commonEdge);
    if(splitFace){
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
                SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                                    inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
                cylinderFace->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)){
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(commonEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 1;
        // Set the assist surface reference to the original surfaces.
        cylinderFace->accessSImpl()->hasAssistSurface = Standard_True;
        commonEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistPlnCylSurfaceGenerator::generateThroughTwoLines(
        const std::shared_ptr<Geometry::BoundSurface>& cylinderFace,
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
                cylinderFace->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)){
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(firstEdge);
        assistSurface->accessBSImpl()->assistEdgesList.push_back(secondEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 2;
        // Set the assist surface reference to the original surfaces.
        cylinderFace->accessSImpl()->hasAssistSurface = Standard_True;
        firstEdge->accessEImpl()->useForSplitSurface = Standard_True;
        secondEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}
