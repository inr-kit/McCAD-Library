//C++
#include <array>
#include <filesystem>
// McCAD
#include "AssistConeSurfaceGenerator.hpp"
#include "CommonEdgeFinder.hpp"
#include "CurveUtilities.hpp"
#include "splitSurfacesGenerator.hpp"
#include "surfaceObjCerator.hpp"
#include "faceParameters.hpp"
#include "coneSolid.hpp"
//OCC
#include <GeomAbs_CurveType.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <TopAbs_Orientation.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <STEPControl_Writer.hxx>
#include <BRepTools.hxx>


McCAD::Decomposition::AssistConeSurfaceGenerator::AssistConeSurfaceGenerator(
    const IO::InputConfig& inputConfig) : inputConfig{ inputConfig } {
}

McCAD::Decomposition::AssistConeSurfaceGenerator::~AssistConeSurfaceGenerator() {
}

/**
 * @brief     Generic assisted surface adding function for cone.
 * @param     Geometry::CONESolid& solidObj
 * @data      7/12/2022
 * @modified  7/12/2022
 * @author    Jiandi Guo
*/
void
McCAD::Decomposition::AssistConeSurfaceGenerator::operator()(
    Geometry::CONESolid& solidObj) {
    auto& conesList = solidObj.accessSImpl()->conesList;

    for (Standard_Integer i = 0; i < conesList.size(); i++) {
        auto face = conesList[i]->accessSImpl()->face;
        auto assistSurface = generateConeAstFace(face);

    }
    auto& cylindersList = solidObj.accessSImpl()->cylindersList;
    auto& planesList = solidObj.accessSImpl()->planesList;
    std::vector<std::shared_ptr<Geometry::Edge>> commonEdges;
    edgesMap commonLineEdgesMap, commonCurveEdgesMap;
    for (Standard_Integer i = 0; i < cylindersList.size(); ++i) {
        // If cylinder is closed, then ignore the cylindrical surface.
        std::cout << "the radian is " << Tools::FaceParameters{}.getRadian(cylindersList[i]->accessSImpl()->face) << " --guo\n";
        //if(Tools::FaceParameters{}.getRadian(cylindersList[i]->accessSImpl()->face) >= 2*inputConfig.PI) continue;
        for (Standard_Integer j = 0; j < planesList.size(); ++j) {
            commonEdges = CommonEdgeFinder{ inputConfig.angularTolerance,
                    inputConfig.distanceTolerance, inputConfig.precision }(
                        cylindersList[i], planesList[j]);
            if (!commonEdges.empty()) {
                for (Standard_Integer k = 0; k < commonEdges.size(); ++k) {
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
                            solidObj.accessSImpl()->rejectSolid = Standard_True;
                        }
                    }
                }
            }
            else if (commonLineEdgesMap.size() == 2) {
                std::vector<std::shared_ptr<Geometry::Edge>> commonEdgesToUse;
                for (const auto& member : commonLineEdgesMap) {
                    if (member.second.size() == 1 // &&
                        // !planesList[member.first]->accessSImpl()->splitSurface &&
                        /* member.second[0]->accessEImpl()->edge.Convex() == 1*/) {
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
                                solidObj.accessSImpl()->rejectSolid = Standard_True;
                            }
                        }
                    }
                    else {
                        // Generate split surfaces through the axis and the edges.
                        Standard_Integer increment{ 0 };
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
                                solidObj.accessSImpl()->rejectSolid = Standard_True;
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
        commonCurveEdgesMap.clear();
    }
}
/**
 * @brief     Generic assisted surface adding function for cone.
 * @param     Geometry::CONESolid& solidObj
 * @data      7/12/2022
 * @modified  7/12/2022
 * @author    Jiandi Guo
*/

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistConeSurfaceGenerator::generateConeAstFace(
    const TopoDS_Face face) 
{
    TopAbs_Orientation Orint;
    Orint = face.Orientation();
    if (Orint == TopAbs_FORWARD) return std::nullopt;

    BRepAdaptor_Surface BS(face, Standard_True);
    GeomAdaptor_Surface adpFace = BS.Surface();

    TopLoc_Location loc;
    Handle_Geom_Surface HndGeomFace = BRep_Tool::Surface(face, loc);

    Standard_Real Umin, Umax, Vmin, Vmax;
    BRepTools::UVBounds(face, Umin, Umax, Vmin, Vmax);

    if (Umin < 1.0e-10) Umin = 0.0;
    if (Umax < 1.0e-10) Umax = 0.0;
    if (Vmin < 1.0e-10) Vmin = 0.0;
    if (Vmax < 1.0e-10) Vmax = 0.0;
    BRepAdaptor_Surface BSF(face, Standard_True);
    //the surface coordinate system
    gp_Trsf T = BSF.Trsf();
    if (fabs(Umax - Umin) <= gp::Resolution() || fabs(Vmax - Vmin) <= gp::Resolution()) return std::nullopt;
    if (adpFace.IsUClosed()) return std::nullopt;
    //get extremal point in the surfaces coord sys.
    gp_Pnt P1 = (HndGeomFace->Value(Umin, Vmin)).Transformed(T);
    gp_Pnt P2 = (HndGeomFace->Value(Umin, Vmax)).Transformed(T);
    gp_Pnt P3 = (HndGeomFace->Value(Umax, Vmin)).Transformed(T);
    gp_Pnt P4 = (HndGeomFace->Value(Umax, Vmax)).Transformed(T);

    //get the middle point
    gp_Pnt P_mid = (HndGeomFace->Value(Umin + (Umax - Umin) / 2.0, Vmin + (Vmax - Vmin) / 2.0)).Transformed(T);
    //the point used for genetating assisting surface
    gp_Pnt P_center;


}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistConeSurfaceGenerator::generateThroughLineAxis(
    const std::shared_ptr<Geometry::BoundSurface>& cylinderFace,
    const std::shared_ptr<Geometry::Edge>& commonEdge,
    const Standard_Real& boxDiagonalLength, const Standard_Real& meshDeflection) {
    BRepAdaptor_Surface surfaceAdaptor(cylinderFace->accessSImpl()->face);
    auto splitFace = SplitSurfaceGenerator{ inputConfig.edgeTolerance,
            inputConfig.precision, inputConfig.angularTolerance }.generatePlaneOnLineAxis(
                surfaceAdaptor.Cylinder(), commonEdge);
    if (splitFace) {
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
            SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
            cylinderFace->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)) {
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(commonEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 1;
        assistSurface->accessSImpl()->isAssistSurface = Standard_True;
        // Set the assist surface reference to the original surfaces.
        cylinderFace->accessSImpl()->hasAssistSurface = Standard_True;
        commonEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistConeSurfaceGenerator::generateThroughTwoLines(
    const std::shared_ptr<Geometry::BoundSurface>& cylinderFace,
    const std::shared_ptr<Geometry::Edge>& firstEdge,
    const std::shared_ptr<Geometry::Edge>& secondEdge,
    const Standard_Real& boxDiagonalLength, const Standard_Real& meshDeflection) {
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
            // Edges are not planar!. Cannot be used to vreate surface.
            return std::nullopt;
        }
    }
    auto splitFace = SplitSurfaceGenerator{ inputConfig.edgeTolerance,
            inputConfig.precision, inputConfig.angularTolerance }.generatePlaneOn2Lines(firstEdge, secondEdge);
    if (splitFace) {
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
            SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
            cylinderFace->accessSImpl()->surfaceNumber * 1000;
        if (assistSurface->accessBSImpl()->generateMesh(meshDeflection)) {
            assistSurface->accessBSImpl()->generateEdges(inputConfig.parameterTolerance);
        }
        assistSurface->accessBSImpl()->assistEdgesList.push_back(firstEdge);
        assistSurface->accessBSImpl()->assistEdgesList.push_back(secondEdge);
        assistSurface->accessSImpl()->throughConcaveEdges += 2;
        assistSurface->accessSImpl()->isAssistSurface = Standard_True;
        // Set the assist surface reference to the original surfaces.
        cylinderFace->accessSImpl()->hasAssistSurface = Standard_True;
        firstEdge->accessEImpl()->useForSplitSurface = Standard_True;
        secondEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}
