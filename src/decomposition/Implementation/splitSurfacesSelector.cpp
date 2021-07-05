// C++
#include <algorithm>
#include <filesystem>
// McCAD
#include "splitSurfacesSelector.hpp"
#include "SurfaceUtilities.hpp"
// OCC
#include <GeomAbs_SurfaceType.hxx>
#include <STEPControl_Writer.hxx>

McCAD::Decomposition::SplitSurfacesSelector::SplitSurfacesSelector(){
}

McCAD::Decomposition::SplitSurfacesSelector::~SplitSurfacesSelector(){
}

void
McCAD::Decomposition::SplitSurfacesSelector::generateSplitFacesList(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList,
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& selectedSplitFacesList){
    // 1st step: Select surfaces that go through 0 boundary surfaces.
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->numberCollidingSurfaces == 0)
            selectedSplitFacesList.push_back(face);
    }
    if(!selectedSplitFacesList.empty()) goto sort;

    // 2nd step: select planar surfaces that go through 0 curved surfaces.
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->numberCollidingCurvedSurfaces == 0
            && face->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Plane))
            selectedSplitFacesList.push_back(face);
    }
    if(!selectedSplitFacesList.empty()) goto sort;

    // 3th step: select cylindrical surfaces.
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Cylinder) &&
                !face->accessSImpl()->hasAssistSurface)
            selectedSplitFacesList.push_back(face);
    }
    if(!selectedSplitFacesList.empty()) goto sort;

    // 4th step: select the other surfaces
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Plane))
            selectedSplitFacesList.push_back(face);
    }
    sort:;
    sortSplitFaces(selectedSplitFacesList);
}

void
McCAD::Decomposition::SplitSurfacesSelector::sortSplitFaces(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList){
    // Use a lambda expression as a comparator:
    // Faces which go through more concave edges have a higher priority.
    // If two faces go through the same number of concave edges, the face with
    // less colliding surfaces has a higher priority.
    auto comparator = [](const std::shared_ptr<Geometry::BoundSurface>& first,
            const std::shared_ptr<Geometry::BoundSurface>& second){
        const auto& fst = *first->accessSImpl();
        const auto& snd = *second->accessSImpl();
        return fst.throughConcaveEdges > snd.throughConcaveEdges
                ||
                (fst.throughConcaveEdges == snd.throughConcaveEdges &&
                 fst.numberCollidingSurfaces < snd.numberCollidingSurfaces)
                ||
                (fst.throughConcaveEdges == snd.throughConcaveEdges &&
                 fst.numberCollidingSurfaces == snd.numberCollidingSurfaces &&
                 fst.numberCollidingCurvedSurfaces < snd.numberCollidingCurvedSurfaces)
                ||
                (fst.throughConcaveEdges == snd.throughConcaveEdges &&
                 fst.numberCollidingSurfaces == snd.numberCollidingSurfaces &&
                 fst.numberCollidingCurvedSurfaces == snd.numberCollidingCurvedSurfaces &&
                 fst.repeatedSurface > snd.repeatedSurface);
    };
    std::sort(splitFacesList.begin(), splitFacesList.end(), comparator);
}
