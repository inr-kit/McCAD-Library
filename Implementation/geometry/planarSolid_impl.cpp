// McCAD
#include "planarSolid_impl.hpp"

McCAD::Geometry::PLSolid::Impl::Impl(){
}

McCAD::Geometry::PLSolid::Impl::~Impl(){
}

void
McCAD::Geometry::PLSolid::Impl::judgeDecomposeSurfaces(Solid::Impl*& solidImpl){
    // Judge whether boundary surfaces of the solid can be used for decomposition.
    std::cout << "judgeDecomposeSurfaces, PLSolid" << std::endl;
    auto& facesList = solidImpl->facesList;
    if (facesList.size() < 2){
        return;
    }
    for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i){
        //std::cout << "i=" << i << std::endl;
        auto iFace = facesList[i]->accessSImpl();
        Standard_Integer positiveFaces = 0;
        Standard_Integer negativeFaces = 0;
        Standard_Integer numberCollidingSurfaces = 0;
        for (Standard_Integer j = 0; j <= facesList.size() - 1; ++j){
            //std::cout << "j=" << i << std::endl;
            auto jFace = facesList[j]->accessSImpl();
            if (i != j && iFace->surfaceNumber != jFace->surfaceNumber){
                Standard_Integer side = 0;
                //std::cout << "facecollision" << std::endl;
                if (facesList[i]->accessBSImpl()->faceCollision(*facesList[j], side)){
                    ++numberCollidingSurfaces;
                    //std::cout << "facecollision True" << std::endl;
                    iFace->splitSurface = Standard_True;
                } else{
                    //std::cout << "facecollision False" << std::endl;
                    //std::cout << "side" << side << std::endl;
                    if (side == 1){
                        ++positiveFaces;
                    } else if (side == -1){
                        ++negativeFaces;
                    }
                }
            }
        }
        if (positiveFaces > 0 && negativeFaces > 0){
            //std::cout << "splitsutface True, pos & neg" << std::endl;
            iFace->splitSurface = Standard_True;
        }
        if (iFace->splitSurface){
            //std::cout << "set collidingsurfaces" << std::endl;
            iFace->numberCollidingSurfaces = numberCollidingSurfaces;
            //std::cout << "adding to split surfaces list" << std::endl;
            solidImpl->splitFacesList.push_back(facesList[i]);
            solidImpl->splitSurface = Standard_True;
        }
    }
}
