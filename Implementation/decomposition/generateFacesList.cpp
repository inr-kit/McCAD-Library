//McCAD
#include "generateFacesList.hpp"
#include "generateFacesList.tpp"

McCAD::Decomposition::FacesListGenerator::FacesListGenerator(){
}

McCAD::Decomposition::FacesListGenerator::~FacesListGenerator(){
}

void
McCAD::Decomposition::FacesListGenerator::mergeSurfaces(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& surfacesList,
        Standard_Real& boxSquareLength){
    //std::cout << "mergeSurfaces" << std::endl;
    if (surfacesList.size() < 2){
        return;
    }
    for (Standard_Integer i = 0; i <= surfacesList.size() - 2; ++i){
        for (Standard_Integer j = i+1; j <= surfacesList.size() - 1; ++j){
            if (*surfacesList[i] == *surfacesList[j]){
                surfacesList[j]->accessSImpl()->surfaceNumber =
                        surfacesList[i]->accessSImpl()->surfaceNumber;
                //std::cout << "*** equal" << std::endl;
                /*
                // Save surfaces to step file for comparison/debugging.
                STEPControl_Writer writer6;
                writer6.Transfer(surfacesList[j]->accessSImpl()->face,
                                 STEPControl_StepModelType::STEPControl_AsIs);
                writer6.Transfer(surfacesList[i]->accessSImpl()->face,
                                 STEPControl_StepModelType::STEPControl_AsIs);
                Standard_Integer kk = 0;
                std::string filename = "../examples/bbox/equalsurfaces";
                std::string suffix = ".stp";
                while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                    ++kk;
                }
                filename += std::to_string(kk);
                filename += suffix;
                writer6.Write(filename.c_str());
                */
                // Test if the two surfaces can be fused.
                if (*surfacesList[i] << *surfacesList[j]){
                    //std::cout << "*** equal fuse" << std::endl;
                    if (surfacesList[i]->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
                        TopoDS_Face newFace = Tools::PlaneFuser{}(
                                    surfacesList[i]->accessSImpl()->face,
                                    surfacesList[j]->accessSImpl()->face);
                        std::shared_ptr<Geometry::BoundSurface> newboundSurface =
                                generateSurface<McCAD::Geometry::PLSolid>(
                                    newFace, boxSquareLength);
                        newboundSurface->accessSImpl()->surfaceNumber =
                                surfacesList[i]->accessSImpl()->surfaceNumber;
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
                        // Erase pointer surfacesList[j] & [i] from surfacesList.
                        surfacesList.erase(surfacesList.begin() + j);
                        --j;
                        surfacesList.erase(surfacesList.begin() + i);
                        --i;
                        surfacesList.push_back(std::move(newboundSurface));
                        break;
                    }
                } else{
                    //std::cout << "*** equal, erase one" << std::endl;
                    // Erase pointer surfacesList[j] from surfacesList.
                    /*
                    STEPControl_Writer writer7;
                    writer7.Transfer(surfacesList[j]->accessSImpl()->face,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    std::string filename = "../examples/bbox/equalsurfacedelete";
                    std::string suffix = ".stp";
                    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                        ++kk;
                    }
                    filename += std::to_string(kk);
                    filename += suffix;
                    writer7.Write(filename.c_str());
                    */
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
