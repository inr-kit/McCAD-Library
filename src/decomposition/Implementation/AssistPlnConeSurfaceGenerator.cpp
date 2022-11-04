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
#include <STEPControl_Writer.hxx>

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
        if (Tools::FaceParameters{}.getRadian(conesList[i]->accessSImpl()->face) >= 2 * inputConfig.PI) continue;
        /*
        for (int j = 0; j < planesList.size(); ++j) {
            commonEdges = CommonEdgeFinder{ inputConfig.angularTolerance,
                    inputConfig.distanceTolerance, inputConfig.precision }(
                        cylindersList[i], planesList[j]);
            if (!commonEdges.empty()) {
                for (int k = 0; k < commonEdges.size(); ++k) {
                    // Only accept edges that are not spot or seem edges
                    if (!BRep_Tool::IsClosed(commonEdges[k]->accessEImpl()->edge,
                        cylindersList[i]->accessSImpl()->face) &&
                        !BRep_Tool::Degenerated(commonEdges[k]->accessEImpl()->edge)) {
                        if (commonEdges[k]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line))
                            commonLineEdgesMap[j].push_back(commonEdges[k]);
                        else commonCurveEdgesMap[j].push_back(commonEdges[k]);
                    }
                }
            }
        }
        if (!commonLineEdgesMap.empty()) {
            // The Cylinder has common line edges with planes.
            if (commonLineEdgesMap.size() == 1) {
                // The Cylinder has common line edges with a single plane.
                for (const auto& member : commonLineEdgesMap) {
                    //if(!planesList[member.first]->accessSImpl()->splitSurface) continue;
                    if (member.second.size() == 1) {
                        // The cylinder has a single common line edge with a single plane.
                        auto assistSurface = generateThroughLineAxis(
                            cylindersList[i], member.second[0],
                            solidObj.accessSImpl()->boxDiagonalLength,
                            solidObj.accessSImpl()->meshDeflection);
                        if (assistSurface) {
                            solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                            solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                            solidObj.accessSImpl()->assistFacesMap[planesList[member.first]] = assistSurface.value();
                        }
                        else {
                            // If there exists a common edge between cylindrical surfaces,
                            // but failed to generate a split surface then reject solid.
                            solidObj.accessSImpl()->rejectSolid = true;
                        }
                    }
                }
            }
            else if (commonLineEdgesMap.size() == 2) {
                std::vector<std::shared_ptr<Geometry::Edge>> commonEdgesToUse;
                for (const auto& member : commonLineEdgesMap) {
                    if (member.second.size() == 1 // &&
                        // !planesList[member.first]->accessSImpl()->splitSurface &&
                        /* member.second[0]->accessEImpl()->edge.Convex() == 1*//*) {
                        commonEdgesToUse.push_back(member.second[0]);
                    }
                }
                if (cylindersList[i]->accessSImpl()->face.Orientation() == TopAbs_REVERSED) {
                    // Cylinder is concave.
                    goto useBothEdges;
                }
                else if (cylindersList[i]->accessSImpl()->face.Orientation() == TopAbs_FORWARD) {
                    // Cylinder is convex.
                    if (Tools::FaceParameters{}.getRadian(cylindersList[i]->accessSImpl()->face) > 3 * inputConfig.PI / 4.0) {
                    useBothEdges:;
                        // Generate split surface through the two edges.
                        if (commonEdgesToUse.size() == 2) {
                            auto assistSurface = generateThroughTwoLines(
                                cylindersList[i], commonEdgesToUse[0], commonEdgesToUse[1],
                                solidObj.accessSImpl()->boxDiagonalLength,
                                solidObj.accessSImpl()->meshDeflection);
                            if (assistSurface) {
                                solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                                solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                            }
                            else {
                                // If there exists a common edge between cylindrical surfaces,
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
                                    cylindersList[i], edge,
                                    solidObj.accessSImpl()->boxDiagonalLength,
                                    solidObj.accessSImpl()->meshDeflection);
                                if (assistSurface) {
                                    assistSurface.value()->accessSImpl()->surfaceNumber += increment;
                                    solidObj.accessSImpl()->assistFacesList.push_back(assistSurface.value());
                                    solidObj.accessSImpl()->assistFacesMap[cylindersList[i]] = assistSurface.value();
                                }
                                ++increment;
                            }
                            if (!solidObj.accessSImpl()->assistFacesMap[cylindersList[i]]) {
                                // If there exists a common edge between cylindrical surfaces,
                                // but failed to generate a split surface then reject solid.
                                solidObj.accessSImpl()->rejectSolid = true;
                            }
                        }
                    }
                }
            }
        }
        if (!commonCurveEdgesMap.empty()) {
            // Cylinder has common edges with more than one plane.
        }
        commonLineEdgesMap.clear();
        commonCurveEdgesMap.clear();*/
    }
}
