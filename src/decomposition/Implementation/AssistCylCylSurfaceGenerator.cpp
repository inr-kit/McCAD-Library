//C++
#include <math.h>
#include <memory>
#include <vector>
// McCAD
#include "AssistCylCylSurfaceGenerator.hpp"
#include "CommonEdgeFinder.hpp"
#include "EdgesCombiner.hpp"
#include "CurveUtilities.hpp"
#include "splitSurfacesGenerator.hpp"
#include "surfaceObjCerator.hpp"
#include "boundSurface_impl.hpp"
//OCC
#include <Standard.hxx>
#include <GeomAbs_CurveType.hxx>

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
            commonEdges = CommonEdgeFinder{}(cylindersList[i], cylindersList[j]);
            if(commonEdges.size() == 1){
                std::cout << "1 common edge(s)" << std::endl;
                if(commonEdges[0]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
                    auto assistSurface = generateThroughLine(
                                cylindersList[i], cylindersList[j], commonEdges[0],
                                solidObj.accessSImpl()->boxDiagonalLength);
                    if(assistSurface){
                        solidObj.accessSImpl()->splitAssistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]]= assistSurface.value();
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[j]]= assistSurface.value();
                    }
                }
            } else if (commonEdges.size() == 2){
                if(commonEdges[0]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)
                   && commonEdges[1]->accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
                    auto assistSurface = generateThroughTwoLines(
                                cylindersList[i], cylindersList[j], commonEdges[0],
                                commonEdges[1], solidObj.accessSImpl()->boxDiagonalLength);
                    if(assistSurface){
                        solidObj.accessSImpl()->splitAssistFacesList.push_back(assistSurface.value());
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[i]]= assistSurface.value();
                        solidObj.accessSImpl()->assistFacesMap[cylindersList[j]]= assistSurface.value();
                    }
                }
            } else{
                std::cout << "more common edges" << std::endl;
            }
        }
    }
}

std::optional<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::AssistCylCylSurfaceGenerator::generateThroughLine(
        const std::shared_ptr<Geometry::BoundSurface>& firstFace,
        const std::shared_ptr<Geometry::BoundSurface>& secondFace,
        const std::shared_ptr<Geometry::Edge>& commonEdge,
        const Standard_Real& boxDiagonalLength){
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance}.generatePlaneOnLine(
                firstFace->accessSImpl()->face, secondFace->accessSImpl()->face,
                commonEdge);
    if(splitFace){
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
                SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                                    inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
                firstFace->accessSImpl()->surfaceNumber * 1000;
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
        const Standard_Real& boxDiagonalLength){
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
    auto splitFace = SplitSurfaceGenerator{inputConfig.edgeTolerance}.generatePlaneOn2Lines(
                firstEdge, secondEdge);
    if(splitFace){
        std::shared_ptr<Geometry::BoundSurface> assistSurface =
                SurfaceObjCreator{}(splitFace.value(), boxDiagonalLength,
                                    inputConfig.edgeTolerance);
        assistSurface->accessSImpl()->surfaceNumber =
                firstFace->accessSImpl()->surfaceNumber * 1000;
        // Set the assist surface reference to the original surfaces.
        firstFace->accessSImpl()->hasAssistSurface = Standard_True;
        secondFace->accessSImpl()->hasAssistSurface = Standard_True;
        firstEdge->accessEImpl()->useForSplitSurface = Standard_True;
        secondEdge->accessEImpl()->useForSplitSurface = Standard_True;
        return assistSurface;
    }
    return std::nullopt;
}
