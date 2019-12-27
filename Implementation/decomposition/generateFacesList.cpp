//McCAD
#include "generateFacesList.hpp"
#include "generateFacesList.tpp"

McCAD::Decomposition::FacesListGenerator::FacesListGenerator(){
}

McCAD::Decomposition::FacesListGenerator::~FacesListGenerator(){
}

void
McCAD::Decomposition::FacesListGenerator::mergeSurfaces(
        std::vector<BS>& surfacesList,
        Standard_Real& boxSquareLength){
    //std::cout << "mergeSurfaces" << std::endl;
    if (surfacesList.size() < 2){
        return;
    }
    for (Standard_Integer i = 0; i < surfacesList.size(); ++i){
        for (Standard_Integer j = i+1; j < surfacesList.size(); ++j){
            if (*surfacesList[i] == *surfacesList[j]){
                surfacesList[j]->accessSImpl()->surfaceNumber =
                        surfacesList[i]->accessSImpl()->surfaceNumber;
                 //std::cout << "*** equal" << std::endl;
                // Test if the two surfaces can be fused.
                if (*surfacesList[i] << *surfacesList[j]){
                    //std::cout << "*** equal, fuse" << std::endl;
                    TopoDS_Face newFace = Tools::SurfacesFuser{}(
                                surfacesList[i]->accessSImpl()->face,
                                surfacesList[j]->accessSImpl()->face).value();
                    BS newboundSurface =
                            FacesListGenerator{}(newFace, boxSquareLength);
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
                    //std::cout << "combin edges" << std::endl;
                    //std::cout << "edges list of surface i: " <<
                    //             surfacesList[i]->accessBSImpl()->edgesList.size()
                    //          << std::endl;
                    newboundSurface->accessBSImpl()->combineEdges(
                                surfacesList[i]->accessBSImpl()->edgesList);
                    //std::cout << "combined edges of surface i" << std::endl;
                    //std::cout << "edges list of surface j: " <<
                    //             surfacesList[j]->accessBSImpl()->edgesList.size()
                    //          << std::endl;
                    newboundSurface->accessBSImpl()->combineEdges(
                                surfacesList[j]->accessBSImpl()->edgesList);
                    //std::cout << "combined edges of surface j" << std::endl;
                    // Erase pointer surfacesList[j] & [i] from surfacesList.
                    //std::cout << "erase pointers to surfaces i and j" << std::endl;
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
                if (surfacesList.size() < 2){
                    return;
                }
            }
        }
    }
}
