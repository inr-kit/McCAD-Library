// McCAD
#include "cylSolid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "edgeOnSurface.hpp"
#include "faceCollision.hpp"
#include "SurfaceUtilities.hpp"
#include "EdgeType.hpp"
// OCCT
#include <GeomAbs_SurfaceType.hxx>
#include <STEPControl_Writer.hxx>
#include <filesystem>

/** ********************************************************************
* @brief    A function that judges if any boundary surface can be used to split the solid.
* @param    solidImpl is a McCAD solid object.
* @param    precision is used to compare numerical values and is set in the inputConfig file.
* @param    distanceTolerance is used in edges comparison by OCCT and is set in the inputConfig file.
* @date     31/12/2020
* @modified 
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Geometry::CYLSolid::Impl::judgeDecomposeSurfaces(Solid::Impl* solidImpl,
                                                        double precision,
                                                        double distanceTolerance){
    // Judge whether boundary surfaces of the solid can be used for decomposition.
    auto& facesList = solidImpl->facesList;
    if (facesList.size() < 2) return;
    for (int i = 0; i < facesList.size(); ++i){
        auto iFace = facesList[i]->accessSImpl();
        int positiveFaces{0}, negativeFaces{0},
                         numberCollidingSurfaces{0},
                         numberCollidingCurvedSurfaces{0};
        for (int j = 0; j < facesList.size(); ++j){
            auto jFace = facesList[j]->accessSImpl();
            if (i != j && iFace->surfaceNumber != jFace->surfaceNumber){
                int side{0};
                if (Decomposition::FaceCollision{precision, distanceTolerance}.operator()(
                            *facesList[i], *facesList[j], side)){
                    ++numberCollidingSurfaces;
                    iFace->splitSurface = true;
                    if (jFace->surfaceType != Tools::toTypeName(GeomAbs_Plane)){
                        ++numberCollidingCurvedSurfaces;
                    }
                } else{
                    if (side == 1) ++positiveFaces;
                    else if (side == -1) ++negativeFaces;
                }
            }
        }
        if (positiveFaces > 0 && negativeFaces > 0){
            iFace->splitSurface = true;
        }
        if (iFace->splitSurface){
            iFace->numberCollidingSurfaces = numberCollidingSurfaces;
            iFace->numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
            solidImpl->splitFacesList.push_back(facesList[i]);
            solidImpl->splitSurface = true;
        }
    }
}

void
McCAD::Geometry::CYLSolid::Impl::judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl,
                                                              double precision,
                                                              double distanceTolerance){
    // Judge whether boundary surfaces of the solid can be used for decomposition.
    auto& firstfacesList = solidImpl->assistFacesList;
    auto& secondfacesList = solidImpl->facesList;
    if (firstfacesList.size() < 1 || secondfacesList.size() < 1) return;
    for (int i = 0; i < firstfacesList.size(); ++i){
        auto iFace = firstfacesList[i]->accessSImpl();
        int positiveFaces{0}, negativeFaces{0},
                         numberCollidingSurfaces{0},
                         numberCollidingCurvedSurfaces{0};
        for (int j = 0; j < secondfacesList.size(); ++j){
            int side{0};
            if (Decomposition::FaceCollision{precision, distanceTolerance}.operator()(
                        *firstfacesList[i], *secondfacesList[j], side)){
                ++numberCollidingSurfaces;
                iFace->splitSurface = true;
                if (secondfacesList[j]->accessSImpl()->surfaceType != Tools::toTypeName(GeomAbs_Plane)){
                    ++numberCollidingCurvedSurfaces;
                }
            } else{
                if (side == 1) ++positiveFaces;
                else if (side == -1) ++negativeFaces;
            }
        }
        if (positiveFaces > 0 && negativeFaces > 0){
            iFace->splitSurface = true;
        }
        if (iFace->splitSurface){
            iFace->numberCollidingSurfaces = numberCollidingSurfaces;
            iFace->numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
            solidImpl->splitAssistFacesList.push_back(firstfacesList[i]);
            solidImpl->splitFacesList.push_back(firstfacesList[i]);
            solidImpl->splitSurface = true;
        }
    }
}

void
McCAD::Geometry::CYLSolid::Impl::judgeThroughConcaveEdges(Solid::Impl* solidImpl, 
                                                          const double& distanceTolerance){
    // Judge how many concave edges each boundary face of solid goes through.
    auto& facesList = solidImpl->splitFacesList;
    if (facesList.size() < 2) return;
    for (int i = 0; i < facesList.size(); ++i){
        // Don't update throughConcaveEdges if it already has a value.
        // Only update surfaces that result from fusing others.
        if (facesList[i]->accessSImpl()->throughConcaveEdges != 0) continue;
        int throughConcaveEdges = 0;
        for (int j = 0; j < facesList.size(); ++j){
            if (i != j && facesList[i]->accessSImpl()->surfaceNumber !=
                    facesList[j]->accessSImpl()->surfaceNumber){
                auto& edgesList = facesList[j]->accessBSImpl()->edgesList;
                for (int k = 0; k < edgesList.size(); ++k){
                    if (edgesList[k]->accessEImpl()->convexity == Tools::EdgeType{}.concave &&
                            Decomposition::EdgeOnSurface{}(
                                facesList[i]->accessSImpl()->face, *(edgesList[k]), distanceTolerance)){
                        ++throughConcaveEdges;
                    }
                }
            }
        }
        facesList[i]->accessSImpl()->throughConcaveEdges = throughConcaveEdges;
    }
}

