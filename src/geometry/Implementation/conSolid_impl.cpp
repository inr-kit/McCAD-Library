// McCAD
#include "conSolid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "edgeOnSurface.hpp"
#include "faceCollision.hpp"
#include "SurfaceUtilities.hpp"

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
McCAD::Geometry::CONSolid::Impl::judgeDecomposeSurfaces(Solid::Impl* solidImpl,
                                                        double precision,
                                                        double distanceTolerance) {
    // Judge whether boundary surfaces of the solid can be used for decomposition.
    auto& facesList = solidImpl->facesList;
    if (facesList.size() < 2) return;
    for (int i = 0; i < facesList.size(); ++i) {
        auto iFace = facesList[i]->accessSImpl();
        int positiveFaces{ 0 }, negativeFaces{ 0 },
            numberCollidingSurfaces{ 0 },
            numberCollidingCurvedSurfaces{ 0 };
        for (int j = 0; j < facesList.size(); ++j) {
            auto jFace = facesList[j]->accessSImpl();
            if (i != j && iFace->surfaceNumber != jFace->surfaceNumber) {
                int side{ 0 };
                if (Decomposition::FaceCollision{ precision, distanceTolerance }.operator()(
                    *facesList[i], *facesList[j], side)) {
                    ++numberCollidingSurfaces;
                    iFace->splitSurface = true;
                    if (jFace->surfaceType != Tools::toTypeName(GeomAbs_Plane)) {
                        ++numberCollidingCurvedSurfaces;
                    }
                }
                else {
                    if (side == 1) ++positiveFaces;
                    else if (side == -1) ++negativeFaces;
                }
            }
        }
        if (positiveFaces > 0 && negativeFaces > 0) {
            iFace->splitSurface = true;
        }
        if (iFace->splitSurface) {
            iFace->numberCollidingSurfaces = numberCollidingSurfaces;
            iFace->numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
            solidImpl->splitFacesList.push_back(facesList[i]);
            solidImpl->splitSurface = true;
        }
    }
}

