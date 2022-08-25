// McCAD
#include "surfacesMerger.hpp"
#include "surfaceComparator.hpp"
#include "surfaceObjCerator.hpp"
#include "surfacesFuser.hpp"
#include "EdgesComparator.hpp"
//OCC
#include <TopoDS_Face.hxx>

/** ********************************************************************
* @brief    An operator that merges McCAD surface objects.
* @param    surfacesList is a vector of McCAD surface objects of the same type.
* @param    boxDiagonalLength is the diagonal of the solid BB.
* @param    precision is used in comparing numerical values and is set on the inputConfig file.
* @param    edgeTolerance is used in comparing OCCT edges and is set on the inputConfig file.
* @param    angularTolerance is used in comparing angles and is set on the inputConfig file.
* @param    distanceTolerance is used in comparing distances and is set on the inputConfig file.  
* @date     31/12/2020
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Decomposition::SurfacesMerger::operator()(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& surfacesList,
        const double& boxDiagonalLength,
        const double& precision,
        const double& edgeTolerance,
        const double& angularTolerance,
        const double& distanceTolerance){
    if (surfacesList.size() < 2) return;
    for (int i = 0; i < surfacesList.size() - 1; ++i){
        for (int j = i+1; j < surfacesList.size(); ++j){
            // Test if the two surfaces are equal.
            if(Tools::SurfaceComparator{precision, angularTolerance, distanceTolerance}(
                                        surfacesList[i]->accessSImpl()->face,
                                        surfacesList[j]->accessSImpl()->face)){
                surfacesList[j]->accessSImpl()->surfaceNumber =
                        surfacesList[i]->accessSImpl()->surfaceNumber;
                // Test if the two surfaces can be fused.
                if (*surfacesList[i] << *surfacesList[j]){
                    TopoDS_Face newFace = Tools::SurfacesFuser{precision, edgeTolerance}(
                                surfacesList[i]->accessSImpl()->face,
                                surfacesList[j]->accessSImpl()->face).value();
                    std::shared_ptr<Geometry::BoundSurface> newboundSurface =
                            SurfaceObjCreator{}(newFace, boxDiagonalLength, edgeTolerance);
                    newboundSurface->accessSImpl()->surfaceNumber =
                            surfacesList[i]->accessSImpl()->surfaceNumber;
                    // Add triangles of surface i.
                    for (int k = 0; k <=
                         surfacesList[i]->accessBSImpl()->meshTrianglesList.size() - 1; ++k){
                        newboundSurface->accessBSImpl()->meshTrianglesList.push_back(
                                    std::move(surfacesList[i]->accessBSImpl()->meshTrianglesList[k]));
                    }
                    // Add triangles of surface j.
                    for (int k = 0; k <=
                         surfacesList[j]->accessBSImpl()->meshTrianglesList.size() - 1; ++k){
                        newboundSurface->accessBSImpl()->meshTrianglesList.push_back(
                                    std::move(surfacesList[j]->accessBSImpl()->meshTrianglesList[k]));
                    }
                    // Combine edges.
                    newboundSurface->accessBSImpl()->combineEdges(
                                surfacesList[i]->accessBSImpl()->edgesList);
                    newboundSurface->accessBSImpl()->combineEdges(
                                surfacesList[j]->accessBSImpl()->edgesList);
                    surfacesList.erase(surfacesList.begin() + j);
                    --j;
                    surfacesList.erase(surfacesList.begin() + i);
                    --i;
                    surfacesList.push_back(std::move(newboundSurface));
                    break;
                } else{
                    surfacesList[i]->accessSImpl()->repeatedSurface += 1;
                    surfacesList[i]->accessSImpl()->throughConcaveEdges +=
                            surfacesList[j]->accessSImpl()->throughConcaveEdges;
                    surfacesList.erase(surfacesList.begin() + j);
                    --j;
                }
                if (surfacesList.size() < 2) return;
            }
        }
    }
}
