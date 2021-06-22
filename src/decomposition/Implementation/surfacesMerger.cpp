// McCAD
#include "surfacesMerger.hpp"
#include "surfaceComparator.hpp"
#include "surfaceObjCerator.hpp"
#include "surfacesFuser.hpp"
#include "EdgesComparator.hpp"
//OCC
#include <TopoDS_Face.hxx>

void
McCAD::Decomposition::SurfacesMerger::operator()(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& surfacesList,
        const Standard_Real& boxDiagonalLength,
        const Standard_Real& precision,
        const Standard_Real& edgeTolerance,
        const Standard_Real& angularTolerance,
        const Standard_Real& distanceTolerance){
    if (surfacesList.size() < 2) return;
    for (Standard_Integer i = 0; i <= surfacesList.size() - 2; ++i){
        for (Standard_Integer j = i+1; j <= surfacesList.size() - 1; ++j){
            if(Tools::SurfaceComparator{precision, angularTolerance, distanceTolerance}(
                        surfacesList[i]->accessSImpl()->face,
                        surfacesList[j]->accessSImpl()->face)){
                surfacesList[j]->accessSImpl()->surfaceNumber =
                        surfacesList[i]->accessSImpl()->surfaceNumber;
                // Test if the two surfaces can be fused.
                if (*surfacesList[i] << *surfacesList[j]){
                    //std::cout << "*** equal, fuse" << std::endl;
                    TopoDS_Face newFace = Tools::SurfacesFuser{}(
                                surfacesList[i]->accessSImpl()->face,
                                surfacesList[j]->accessSImpl()->face).value();
                    std::shared_ptr<Geometry::BoundSurface> newboundSurface =
                            SurfaceObjCreator{}(newFace, boxDiagonalLength, edgeTolerance);
                    newboundSurface->accessSImpl()->surfaceNumber =
                            surfacesList[i]->accessSImpl()->surfaceNumber;
                    /* //debug
                    STEPControl_Writer writer2;
                    writer2.Transfer(surfacesList[i]->accessSImpl()->face,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    writer2.Transfer(surfacesList[j]->accessSImpl()->face,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    writer2.Transfer(newFace,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    Standard_Integer kk = 0;
                    std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/bbox/surface";
                    std::string suffix = ".stp";
                    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                        ++kk;
                    }
                    filename += std::to_string(kk);
                    filename += suffix;
                    writer2.Write(filename.c_str());
                    */ //debug
                    // Add triangles of surface i.
                    for (Standard_Integer k = 0; k <=
                         surfacesList[i]->accessBSImpl()->meshTrianglesList.size() - 1; ++k){
                        newboundSurface->accessBSImpl()->meshTrianglesList.push_back(
                                    std::move(surfacesList[i]->accessBSImpl()->meshTrianglesList[k]));
                    }
                    // Add triangles of surface j.
                    for (Standard_Integer k = 0; k <=
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
                    //std::cout << "*** equal, erase one" << std::endl;
                    surfacesList.erase(surfacesList.begin() + j);
                    --j;
                }
                if (surfacesList.size() < 2) return;
            }
        }
    }
}
