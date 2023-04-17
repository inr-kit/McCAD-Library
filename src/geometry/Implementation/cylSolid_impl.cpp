// McCAD
#include "cylSolid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "edgeOnSurface.hpp"
#include "faceCollision.hpp"
#include "SurfaceUtilities.hpp"
// OCC
#include <GeomAbs_SurfaceType.hxx>
#include <STEPControl_Writer.hxx>
#include <filesystem>
#include <BRepBndLib.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>

void
McCAD::Geometry::CYLSolid::Impl::judgeDecomposeSurfaces(Solid::Impl* solidImpl,
                                                        Standard_Real precision,
                                                        Standard_Real distanceTolerance){
    // Judge whether boundary surfaces of the solid can be used for decomposition.
    auto& facesList = solidImpl->facesList;
    if (facesList.size() < 2) return;
    for (Standard_Integer i = 0; i < facesList.size(); ++i){
        auto iFace = facesList[i]->accessSImpl();
        Standard_Integer positiveFaces{0}, negativeFaces{0},
                         numberCollidingSurfaces{0},
                         numberCollidingCurvedSurfaces{0};
        for (Standard_Integer j = 0; j < facesList.size(); ++j){
            auto jFace = facesList[j]->accessSImpl();
            if (i != j && iFace->surfaceNumber != jFace->surfaceNumber){
                Standard_Integer side{0};
                if (Decomposition::FaceCollision{precision, distanceTolerance}.operator()(
                            *facesList[i], *facesList[j], side)){
                    ++numberCollidingSurfaces;
                    iFace->splitSurface = Standard_True;
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
            iFace->splitSurface = Standard_True;
        }
        if (iFace->splitSurface){
            iFace->numberCollidingSurfaces = numberCollidingSurfaces;
            iFace->numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
            solidImpl->splitFacesList.push_back(facesList[i]);
            solidImpl->splitSurface = Standard_True;
        }
    }
}

void
McCAD::Geometry::CYLSolid::Impl::judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl,
                                                              Standard_Real precision,
                                                              Standard_Real distanceTolerance){
    // Judge whether boundary surfaces of the solid can be used for decomposition.
    auto& firstfacesList = solidImpl->assistFacesList;
    auto& secondfacesList = solidImpl->facesList;
    if (firstfacesList.size() < 1 || secondfacesList.size() < 1) return;
    for (Standard_Integer i = 0; i < firstfacesList.size(); ++i){
        auto iFace = firstfacesList[i]->accessSImpl();
        //auto& face = firstfacesList[i]->accessSImpl()->face;
        //BRepAdaptor_Surface surfaceAdaptor(face);
        //if (surfaceAdaptor.GetType() == GeomAbs_Plane)
        //{
        //    //auto& extendedFace = firstfacesList[i]->accessSImpl()->extendedFace;
        //    for (TopExp_Explorer ex(face, TopAbs_VERTEX); ex.More(); ex.Next()) {
        //        std::cout << "1" << std::endl;
        //        TopoDS_Vertex vertex = TopoDS::Vertex(ex.Current());
        //        gp_Pnt p = BRep_Tool::Pnt(vertex);
        //        std::cout << "x:" << p.X() << " y:" << p.Y() << " Z:" << p.Z() << "  --guo\n";
        //    }
        //    Bnd_Box boxFace;
        //    BRepBndLib::Add(face, boxFace);
        //    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
        //    boxFace.Get(xmin, ymin, zmin, xmax, ymax, zmax);
        //    std::cout << "x: " << xmin << " " << xmax << " y: " << ymin << " " << ymax << " z: " << zmin << " " << zmax << " --guo\n";
        //    /*std::array<Standard_Real, 4> parameters;
        //    gp_Pln plane = surfaceAdaptor.Plane();
        //    plane.Coefficients(parameters[0], parameters[1], parameters[2], parameters[3]);
        //     std::cout<<"A: "<<parameters[0]<<" B: "<<parameters[1]<<" C: "<<parameters[2]<<" D: "<<parameters[3]<<std::endl;*/
        //}
        Standard_Integer positiveFaces{0}, negativeFaces{0},
                         numberCollidingSurfaces{0},
                         numberCollidingCurvedSurfaces{0};
        for (Standard_Integer j = 0; j < secondfacesList.size(); ++j){
            Standard_Integer side{0};
            if (Decomposition::FaceCollision{precision, distanceTolerance}.operator()(
                        *firstfacesList[i], *secondfacesList[j], side)){
                ++numberCollidingSurfaces;
                iFace->splitSurface = Standard_True;
                if (secondfacesList[j]->accessSImpl()->surfaceType != Tools::toTypeName(GeomAbs_Plane)){
                    ++numberCollidingCurvedSurfaces;
                }
            } else{
                if (side == 1) ++positiveFaces;
                else if (side == -1) ++negativeFaces;
            }
        }
        if (positiveFaces > 0 && negativeFaces > 0){
            iFace->splitSurface = Standard_True;
        }
        if (iFace->splitSurface){
            iFace->numberCollidingSurfaces = numberCollidingSurfaces;
            iFace->numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
            solidImpl->splitAssistFacesList.push_back(firstfacesList[i]);
            solidImpl->splitFacesList.push_back(firstfacesList[i]);
            solidImpl->splitSurface = Standard_True;
        }
    }
}

void
McCAD::Geometry::CYLSolid::Impl::judgeThroughConcaveEdges(
        Solid::Impl* solidImpl){
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
                for (Standard_Integer k = 0; k < edgesList.size(); ++k){
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

