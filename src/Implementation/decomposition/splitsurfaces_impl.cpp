// McCAD
#include "splitsurfaces_impl.hpp"
// C++
#include <algorithm>

void
McCAD::Decomposition::SplitSurfaces::Impl::generateSplitFacesList(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList,
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& selectedSplitFacesList){

    // 1st step: Select surfaces that go through 0 boundary surfaces.
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->numberCollidingSurfaces == 0)
            selectedSplitFacesList.push_back(face);
    }

    if(!selectedSplitFacesList.empty()){
        sortSplitFaces(selectedSplitFacesList);
        return;
    }

    // 2nd step: select surfaces that go through 0 curved surfaces.
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->numberCollidingCurvedSurfaces == 0
            && face->accessSImpl()->surfaceType == "Plane")
            selectedSplitFacesList.push_back(face);
    }

    if(!selectedSplitFacesList.empty()){
        sortSplitFaces(selectedSplitFacesList);
        return;
    }

    // 3th step: for cylinders

    // 4th step: select the other surfaces
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->surfaceType == "Plane")
            selectedSplitFacesList.push_back(face);
    }

    if (!selectedSplitFacesList.empty()){
        sortSplitFaces(selectedSplitFacesList);
    }
}

void
McCAD::Decomposition::SplitSurfaces::Impl::sortSplitFaces(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList){

    // Use a lambda expression as a comparator:
    // Faces which go through more concave edges have a higher priority.
    // If two faces go through the same amount of concave edges,
    // the face with less colliding surfaces has a higher priority.
    auto comparator
            = [](
            const std::shared_ptr<Geometry::BoundSurface>& first,
            const std::shared_ptr<Geometry::BoundSurface>& second){
        const auto& fst = *first->accessSImpl();
        const auto& snd = *second->accessSImpl();
        return fst.throughConcaveEdges > snd.throughConcaveEdges
                ||
                (fst.throughConcaveEdges == snd.throughConcaveEdges
                 &&
                 fst.numberCollidingSurfaces < snd.numberCollidingSurfaces);
    };

    std::sort(splitFacesList.begin(), splitFacesList.end(), comparator);

//    for(const auto& face : splitFacesList)
//        std::cout << "throughConcaveEdges: "
//                  << face->accessSImpl()->throughConcaveEdges
//                  << ", numberCollidingSurfaces: "
//                  << face->accessSImpl()->numberCollidingSurfaces
//                  << std::endl;

}
