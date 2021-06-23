// McCAD
#include "planarSolid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "edgeOnSurface.hpp"
#include "faceCollision.hpp"
#include "SurfaceUtilities.hpp"

void
McCAD::Geometry::PLSolid::Impl::judgeDecomposeSurfaces(Solid::Impl*& solidImpl){
    // Judge whether boundary surfaces of the solid can be used for decomposition.
    auto& facesList = solidImpl->facesList;
    if (facesList.size() < 2) return;
    for (Standard_Integer i = 0; i < facesList.size(); ++i){
        auto iFace = facesList[i]->accessSImpl();
        Standard_Integer positiveFaces{0}, negativeFaces{0},
                         numberCollidingSurfaces{0};
        for (Standard_Integer j = 0; j < facesList.size(); ++j){
            auto jFace = facesList[j]->accessSImpl();
            if (i != j && iFace->surfaceNumber != jFace->surfaceNumber){
                Standard_Integer side = 0;
                if (Decomposition::FaceCollision{}.operator()(
                            *facesList[i], *facesList[j], side)){
                    ++numberCollidingSurfaces;
                    iFace->splitSurface = Standard_True;
                } else{
                    if (side == 1) ++positiveFaces;
                    else if (side == -1) ++negativeFaces;
                }
            }
        }
        if (positiveFaces > 0 && negativeFaces > 0){
            iFace->splitSurface = Standard_True;
        }
        if (iFace->splitSurface){
            iFace->numberCollidingSurfaces = numberCollidingSurfaces;
            solidImpl->splitFacesList.push_back(facesList[i]);
            solidImpl->splitSurface = Standard_True;
        }
    }
}

void
McCAD::Geometry::PLSolid::Impl::judgeThroughConcaveEdges(Solid::Impl*& solidImpl){
    // Judge how many concave edges each boundary face of solid goes through.
    auto& facesList = solidImpl->splitFacesList;
    if (facesList.size() < 2) return;
    for (Standard_Integer i = 0; i < facesList.size(); ++i){
        // Don't update throughConcaveEdges if it already has a value.
        // Only update surfaces that result from fusing others.
        if (facesList[i]->accessSImpl()->throughConcaveEdges != 0) continue;
        Standard_Integer throughConcaveEdges = 0;
        for (Standard_Integer j = 0; j < facesList.size(); ++j){
            if (i != j && facesList[i]->accessSImpl()->surfaceNumber !=
                    facesList[j]->accessSImpl()->surfaceNumber){
                auto& edgesList = facesList[j]->accessBSImpl()->edgesList;
                for (Standard_Integer k = 0; k <= edgesList.size() - 1; ++k){
                    if (edgesList[k]->accessEImpl()->convexity == 0 &&
                            Decomposition::EdgeOnSurface{}(
                                facesList[i]->accessSImpl()->face, *(edgesList[k]))){
                        ++throughConcaveEdges;
                    }
                }
            }
        }
        facesList[i]->accessSImpl()->throughConcaveEdges = throughConcaveEdges;
    }
}
